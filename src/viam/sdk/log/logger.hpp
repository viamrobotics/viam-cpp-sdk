#pragma once

#include <cstdint>

#include <map>
#include <memory>
#include <ostream>

#include <boost/log/attributes/clock.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>

namespace viam {
namespace sdk {

enum class log_level : std::int8_t {
    trace = -2,
    debug = -1,
    info = 0,  // default value is info
    warn = 1,
    error = 2,
    fatal = 3,
};

std::string to_string(log_level);

std::ostream& operator<<(std::ostream&, log_level);

using LogSource = boost::log::sources::severity_channel_logger_mt<log_level>;

std::string global_resource_name();

class Logger {
   public:
    struct Filter {
        Logger* parent;

        bool operator()(const boost::log::attribute_value_set&);
    };

    static Logger& get();

    void set_global_log_level(log_level);

    /// @brief Set the SDK logger severity from a command line argument vector.
    ///
    /// Sets the boost trivial logger's severity to debug if "--log-level=debug" is the
    /// the third argument. Sets severity to info otherwise. Useful for module
    /// implementations. See LogLevel documentation in the RDK for more information on
    /// how to start modules with a "log-level" commandline argument.
    void set_global_log_level(int argc, char** argv);

    void set_resource_log_level(std::string resource, log_level);

    LogSource& logger() {
        return sdk_logger_;
    }

   private:
    friend class RobotClient;
    friend class Instance;
    Logger() = default;

    void init_logging();
    void disable_console_logging();

    LogSource sdk_logger_;
    boost::shared_ptr<boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend>>
        console_sink_;

    log_level global_level_{log_level::info};

    std::map<std::string, log_level> resource_levels_;
};

BOOST_LOG_ATTRIBUTE_KEYWORD_TYPE(attr_channel, "Channel", std::string);
BOOST_LOG_ATTRIBUTE_KEYWORD_TYPE(attr_sev, "Severity", viam::sdk::log_level);
BOOST_LOG_ATTRIBUTE_KEYWORD_TYPE(attr_file, "file", const char*);
BOOST_LOG_ATTRIBUTE_KEYWORD_TYPE(attr_line, "line", unsigned int);
BOOST_LOG_ATTRIBUTE_KEYWORD_TYPE(attr_time,
                                 "TimeStamp",
                                 boost::log::attributes::local_clock::value_type);

// Logging macro for general SDK logs
#define VIAM_LOG_IMPL(lg, level)                                            \
    BOOST_LOG_SEV((lg), ::viam::sdk::log_level::level)                      \
        << ::boost::log::add_value(::viam::sdk::attr_file_type{}, __FILE__) \
        << ::boost::log::add_value(::viam::sdk::attr_line_type{}, __LINE__)

#define VIAM_LOG(level) VIAM_LOG_IMPL(::viam::sdk::Logger::get().logger(), level)

#define VIAM_RESOURCE_LOG(level) VIAM_LOG_IMPL(this->logger_, level)

}  // namespace sdk
}  // namespace viam
