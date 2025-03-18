#include <viam/sdk/log/logging.hpp>

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
#include <viam/sdk/log/private/log_backend.hpp>

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

log_level level_from_string(std::string level) {
    using ll = log_level;

    std::transform(level.begin(), level.end(), level.begin(), ::tolower);

    if (level == "info") {
        return ll::info;
    } else if (level == "warn" || level == "warning") {
        return ll::warn;
    } else if (level == "error") {
        return ll::error;
    } else if (level == "debug") {
        return ll::debug;
    } else if (level == "trace") {
        return ll::trace;
    } else if (level == "fatal") {
        return ll::fatal;
    }

    VIAM_LOG(warn) << "Returning unknown log level `" << level << "` as info";
    return ll::info;
}

std::ostream& operator<<(std::ostream& os, log_level lvl) {
    os << to_string(lvl);
    return os;
}

const char* global_resource_name() {
    return "Viam C++ SDK";
}

bool LogManager::Filter::operator()(const boost::log::attribute_value_set& attrs) const {
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

LogManager& LogManager::get() {
    static LogManager& result = Instance::current(Instance::Creation::open_existing).impl_->log_mgr;

    return result;
}

LogSource& LogManager::global_logger() {
    return sdk_logger_;
}

void LogManager::set_global_log_level(log_level lvl) {
    global_level_ = lvl;
}

void LogManager::set_global_log_level(int argc, char** argv) {
    if (argc >= 3 && strcmp(argv[2], "--log-level=debug") == 0) {
        set_global_log_level(log_level::debug);
    }
}

void LogManager::set_resource_log_level(const std::string& resource, log_level lvl) {
    resource_levels_[resource] = lvl;
}

void LogManager::init_logging() {
    sdk_logger_.channel(global_resource_name());
    boost::log::core::get()->add_global_attribute("TimeStamp",
                                                  boost::log::attributes::local_clock());

    const boost::log::formatter fmt =
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
    VIAM_LOG(debug) << "Initialized console logging";
}

void LogManager::disable_console_logging() {
    VIAM_LOG(debug) << "Disabling console logging";

    // Set a filter which ignores all console logs unless they contain a console force flag which is
    // set to true.
    console_sink_->set_filter(
        [filter = Filter{this}](const boost::log::attribute_value_set& attrs) {
            auto force = attrs[impl::attr_console_force_type{}];
            if (force && *force) {
                return filter(attrs);
            }

            return false;
        });
}

namespace log_detail {

boost::string_view trim_filename(const char* file) {
    const boost::string_view result(file);
    const std::size_t second_last = result  //
                                        .substr(0, result.find_last_of('/'))
                                        .find_last_of('/');

    if (second_last != boost::string_view::npos) {
        return result.substr(second_last + 1, result.size() - second_last);
    }

    return result;
}

}  // namespace log_detail

}  // namespace sdk
}  // namespace viam
