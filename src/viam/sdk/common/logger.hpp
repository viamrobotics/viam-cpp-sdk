#pragma once

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <cstdint>
#include <string>

namespace viam {
namespace sdk {

class Resource;

class Logger {
   public:
    ~Logger();
    enum class log_level : std::int8_t {
        trace = -2,
        debug = -1,
        info = 0,  // default value is info
        warning = 1,
        error = 2,
        fatal = 3,
    };

    explicit Logger(std::string name);
    explicit Logger(std::string name, log_level level);

    void log(const std::string& log, log_level level) const;
    void debug(const std::string& log) const;
    void error(const std::string& log) const;
    void fatal(const std::string& log) const;
    void info(const std::string& log) const;
    void trace(const std::string& log) const;
    void warning(const std::string& log) const;

    void set_log_level(log_level level);

    std::shared_ptr<boost::log::sources::severity_logger<boost::log::trivial::severity_level>>
    underlying();

   private:
    std::string name_;
    log_level level_;
    struct impl;
    std::unique_ptr<impl> impl_;
};

}  // namespace sdk
}  // namespace viam
