#include <viam/sdk/common/logger.hpp>

#include <atomic>
#include <iostream>

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace viam {
namespace sdk {

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

using ll = log_level;
using bll = logging::trivial::severity_level;

struct Logger::impl {
    impl()
        : logger_(std::make_shared<logging::sources::severity_logger<bll>>()), level_(ll::info) {}
    impl(ll level)
        : logger_(std::make_shared<logging::sources::severity_logger<bll>>()), level_(level) {}

    std::shared_ptr<logging::sources::severity_logger<bll>> logger_;
    ll level_;
};

// Convert file path to only the filename
std::string path_to_filename(const std::string& path) {
    return path.substr(path.find_last_of("/\\") + 1);
}

namespace {
std::atomic<bool> inited = false;
std::atomic<bool> inited_with_ostrm = false;
}  // namespace

std::shared_ptr<Logger> default_logger() {
    static const auto default_logger = [] { return std::make_shared<Logger>("viam-cpp-sdk"); }();
    return default_logger;
}

void init_attributes() {
    logging::core::get()->add_thread_attribute("File", attrs::mutable_constant<std::string>(""));
    logging::core::get()->add_thread_attribute("LogLevel",
                                               attrs::mutable_constant<std::string>(""));
    logging::core::get()->add_thread_attribute("LoggerName",
                                               attrs::mutable_constant<std::string>(""));
    logging::core::get()->add_thread_attribute("Line", attrs::mutable_constant<int>(0));
}

ll Logger::from_string(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    if (str == "info") {
        return ll::info;
    } else if (str == "warn" || str == "warning") {
        return ll::warning;
    } else if (str == "error") {
        return ll::error;
    } else if (str == "debug") {
        return ll::debug;
    } else if (str == "trace") {
        return ll::trace;
    } else if (str == "fatal") {
        return ll::fatal;
    }

    VIAM_SDK_TRIVIAL_CUSTOM_FORMATTED_LOG(
        ll::warning,
        "attempted to convert log level string with unknown level "
            << str << ". defaulting to INFO level");
    return ll::info;
}

void init_logging(std::ostream& strm) {
    if (inited_with_ostrm) {
        return;
    }
    inited_with_ostrm = true;

    // remove existing stdout/stderr logging since we're sending logs to RDK
    logging::core::get()->remove_all_sinks();

    init_attributes();

    // The current use case for init logging
    logging::add_console_log(strm,
                             boost::parameter::keyword<keywords::tag::format>::get() =
                                 (expr::stream << '[' << expr::attr<std::string>("File") << ':'
                                               << expr::attr<int>("Line") << "]    "));
    logging::add_common_attributes();
}

void init_logging() {
    init_attributes();
    if (inited || inited_with_ostrm) {
        return;
    }
    inited = true;

    logging::add_console_log(
        std::clog,
        boost::parameter::keyword<keywords::tag::format>::get() =
            (expr::stream << expr::format_date_time<boost::posix_time::ptime>(
                                 "TimeStamp", "%Y-%m-%d_%H:%M:%S.%f")
                          << ": <" << boost::log::trivial::severity << "> " << '['
                          << expr::attr<std::string>("File") << ':' << expr::attr<int>("Line")
                          << "] " << expr::smessage));
    logging::add_common_attributes();
}

void Logger::set_log_level(ll level) {
    level_ = level;
}

Logger::Logger(std::string name)
    : name_(std::move(name)), level_(log_level::info), impl_(std::make_unique<impl>()) {};
Logger::Logger(std::string name, log_level level)
    : name_(std::move(name)), level_(level), impl_(std::make_unique<impl>(level)) {};

logging::trivial::severity_level _log_level_to_severity_level(log_level level) {
    switch (level) {
        case ll::error: {
            return bll::error;
        }
        case ll::warning: {
            return bll::warning;
        }
        case ll::debug: {
            return bll::debug;
        }
        case ll::fatal: {
            return bll::fatal;
        }
        case ll::trace: {
            return bll::trace;
        }
        case ll::info:  // fallthrough
        default: {
            return bll::info;
        }
    }
}

std::string level_to_string(log_level level) {
    switch (level) {
        case ll::error: {
            return "error";
        }
        case ll::warning: {
            return "warning";
        }
        case ll::debug: {
            return "debug";
        }
        case ll::fatal: {
            return "fatal";
        }
        case ll::trace: {
            return "trace";
        }
        case ll::info: {
            return "info";
        }
    }
}

template <typename ValueType>
ValueType set_get_attrib(const char* name, ValueType value) {
    auto attr = boost::log::attribute_cast<boost::log::attributes::mutable_constant<ValueType>>(
        boost::log::core::get()->get_thread_attributes()[name]);
    attr.set(value);
    return attr.get();
}

void Logger::log(const std::string& msg, log_level level, const char* filename, int line_no) const {
    // in case logging hasn't been initialized, let's set it up.
    // (RSDK-9172) This should be called from within an initializer object that handles all SDK
    // initialization for us.
    init_logging();

    BOOST_LOG_STREAM_WITH_PARAMS(
        *(impl_->logger_),
        (set_get_attrib("LogLevel", level_to_string(level)))(set_get_attrib("LoggerName", name_))(
            set_get_attrib("File", viam::sdk::path_to_filename(filename)))(set_get_attrib(
            "Line", line_no))(boost::log::keywords::severity = _log_level_to_severity_level(level)))
        << msg;
}

void Logger::static_log_(const std::string& msg,
                         log_level level,
                         const char* filename,
                         int line_no) {
    default_logger()->log(msg, level, filename, line_no);
}

Logger::~Logger() = default;

}  // namespace sdk
}  // namespace viam
