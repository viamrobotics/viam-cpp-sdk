#include <viam/sdk/log/logger.hpp>

#include <iostream>

#include <boost/core/null_deleter.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/console.hpp>

#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/common/private/instance.hpp>

namespace viam {
namespace sdk {

std::string to_string(log_level lvl) {
    switch (lvl) {
        case log_level::trace:
            return "trace";
        case log_level::debug:
            return "debug";
        case log_level::info:
            return "info";
        case log_level::warn:
            return "warning";
        // RDK does not support fatal so we send error regardless
        case log_level::error:  // fallthrough
        case log_level::fatal:
            return "error";
        default:
            return std::to_string(static_cast<std::underlying_type_t<log_level>>(lvl));
    }
}

log_level level_from_string(std::string str) {
    using ll = log_level;

    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    if (str == "info") {
        return ll::info;
    } else if (str == "warn" || str == "warning") {
        return ll::warn;
    } else if (str == "error") {
        return ll::error;
    } else if (str == "debug") {
        return ll::debug;
    } else if (str == "trace") {
        return ll::trace;
    } else if (str == "fatal") {
        return ll::fatal;
    }

    VIAM_LOG(warn) << "Returning unknown log level " << str << " as info";
    return ll::info;
}

std::ostream& operator<<(std::ostream& os, log_level lvl) {
    os << to_string(lvl);
    return os;
}

std::string global_resource_name() {
    return "Viam C++ SDK";
}

bool Logger::Filter::operator()(const boost::log::attribute_value_set& attrs) const {
    auto sev = attrs[attr_sev_type{}];
    if (!sev) {
        return false;
    }

    auto resource = attrs[attr_channel_type{}];
    if (resource) {
        auto it = parent->resource_levels_.find(*resource);
        if (it != parent->resource_levels_.end()) {
            return *sev >= it->second;
        }
    }

    return *sev >= parent->global_level_;
}

Logger& Logger::get() {
    static Logger& result = Instance::current(Instance::Creation::open_existing).impl_->logger;

    return result;
}

LogSource& Logger::logger() {
    return sdk_logger_;
}

void Logger::set_global_log_level(log_level lvl) {
    global_level_ = lvl;
}

void Logger::set_global_log_level(int argc, char** argv) {
    if (argc >= 3 && strcmp(argv[2], "--log-level=debug") == 0) {
        set_global_log_level(log_level::debug);
    }
}

void Logger::set_resource_log_level(const std::string& resource, log_level lvl) {
    resource_levels_[resource] = lvl;
}

void Logger::init_logging() {
    sdk_logger_.channel(global_resource_name());
    boost::log::core::get()->add_global_attribute("TimeStamp",
                                                  boost::log::attributes::local_clock());

    boost::log::formatter fmt =
        boost::log::expressions::stream
        << boost::log::expressions::format_date_time<boost::posix_time::ptime>(
               "TimeStamp", "%Y--%m--%d %H:%M:%S")
        << ": [" << attr_channel_type{} << "] <" << attr_sev_type{} << "> [" << attr_file_type{}
        << ":" << attr_line_type{} << "] " << boost::log::expressions::smessage;

    auto backend = boost::make_shared<boost::log::sinks::text_ostream_backend>();
    backend->add_stream(boost::shared_ptr<std::ostream>(&std::cout, boost::null_deleter()));
    backend->auto_flush(true);

    console_sink_ = boost::make_shared<
        boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend>>(backend);

    console_sink_->set_filter(Filter{this});
    console_sink_->set_formatter(fmt);

    boost::log::core::get()->add_sink(console_sink_);
}

void Logger::disable_console_logging() {
    boost::log::core::get()->remove_sink(console_sink_);
}

}  // namespace sdk
}  // namespace viam
