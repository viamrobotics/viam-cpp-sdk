#include <viam/sdk/common/logger.hpp>

#include <iostream>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

namespace viam {
namespace sdk {

namespace logging = boost::log;
using ll = Logger::log_level;
using bll = logging::trivial::severity_level;

struct Logger::impl {
    impl()
        : logger_(std::make_shared<logging::sources::severity_logger<bll>>()), level_(ll::info) {}
    impl(ll level)
        : logger_(std::make_shared<logging::sources::severity_logger<bll>>()), level_(level) {}

    std::shared_ptr<logging::sources::severity_logger<bll>> logger_;
    ll level_;
};

std::shared_ptr<logging::sources::severity_logger<bll>> Logger::underlying() {
    return impl_->logger_;
}

Logger::Logger(std::string name)
    : name_(std::move(name)), level_(log_level::info), impl_(std::make_unique<impl>()) {};
Logger::Logger(std::string name, log_level level)
    : name_(std::move(name)), level_(level), impl_(std::make_unique<impl>(level)) {};

logging::trivial::severity_level _log_level_to_severity_level(Logger::log_level level) {
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

void Logger::log(const std::string& log, log_level level) const {
    if (impl_->level_ > level) {
        return;
    }
    BOOST_LOG_SEV(*(impl_->logger_), _log_level_to_severity_level(level)) << log;
}
void Logger::debug(const std::string& log) const {
    this->log(log, ll::debug);
}
void Logger::error(const std::string& log) const {
    this->log(log, ll::error);
}
void Logger::fatal(const std::string& log) const {
    this->log(log, ll::fatal);
}
void Logger::info(const std::string& log) const {
    this->log(log, ll::info);
}
void Logger::trace(const std::string& log) const {
    this->log(log, ll::trace);
}
void Logger::warning(const std::string& log) const {
    this->log(log, ll::warning);
}

Logger::~Logger() = default;

}  // namespace sdk
}  // namespace viam
