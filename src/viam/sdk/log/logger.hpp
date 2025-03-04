#pragma once

#include <cstdint>

#include <map>
#include <memory>
#include <ostream>

#include <boost/log/sources/severity_channel_logger.hpp>

namespace viam {
namespace sdk {

enum class log_level : std::int8_t {
    trace = -2,
    debug = -1,
    info = 0,  // default value is info
    warning = 1,
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

    void set_global_log_level(log_level);
    void set_resource_log_level(std::string resource, log_level);

    static Logger& get();

   private:
    friend class Instance;
    Logger() = default;

    void init_logging();

    LogSource sdk_logger_;

    log_level global_level_{log_level::info};

    std::map<std::string, log_level> resource_levels_;
};

}  // namespace sdk
}  // namespace viam
