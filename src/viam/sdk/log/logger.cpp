#include <viam/sdk/log/logger.hpp>

#include <iostream>

#include <boost/log/attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/console.hpp>

#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/common/private/instance.hpp>
#include <viam/sdk/log/private/keywords.hpp>

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
        case log_level::warning:
            return "warning";
        case log_level::error:  // fallthrough
        case log_level::fatal:
            return "error";
        default:
            return std::to_string(static_cast<std::underlying_type_t<log_level>>(lvl));
    }
}

std::ostream& operator<<(std::ostream& os, log_level lvl) {
    os << to_string(lvl);
    return os;
}

std::string global_resource_name() {
    return "Viam C++ SDK";
}

bool Logger::Filter::operator()(const boost::log::attribute_value_set& attrs) {
    auto sev = attrs[attr_sev];
    if (!sev) {
        return false;
    }

    auto resource = attrs[attr_channel];
    if (resource) {
        auto it = parent->resource_levels_.find(*resource);
        if (it != parent->resource_levels_.end()) {
            return *sev >= it->second;
        }
    }

    return *sev >= parent->global_level_;
}

Logger& Logger::get() {
    static Logger& result = Instance::current().impl_->logger;

    return result;
}

void Logger::init_logging() {
    sdk_logger_.channel(global_resource_name());

    boost::log::formatter fmt =
        boost::log::expressions::stream
        << boost::log::expressions::format_date_time<boost::posix_time::ptime>(
               "TimeStamp", "%Y--%m--%d %H:%M:%S")
        << ": [" << attr_channel << "] <" << attr_sev << "> [" << attr_file << ":" << attr_line
        << "]" << boost::log::expressions::smessage;

    boost::log::add_console_log(
        std::cout, boost::log::keywords::filter = Filter{this}, boost::log::keywords::format = fmt);

    boost::shared_ptr<boost::log::core> core = boost::log::core::get();

    core->add_global_attribute("TimeStamp", boost::log::attributes::local_clock());
}

}  // namespace sdk
}  // namespace viam
