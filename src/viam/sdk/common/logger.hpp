#pragma once

#include <cstdint>
#include <iostream>
#include <sstream>
#include <memory>
#include <string>

// Some of the logic here has been cribbed from
// https://stackoverflow.com/questions/19415845/a-better-log-macro-using-template-metaprogramming

// Workaround GCC 4.7.2 not recognizing noinline attribute
#ifndef NOINLINE_ATTRIBUTE
#ifdef __ICC
#define NOINLINE_ATTRIBUTE __attribute__((noinline))
#else
#define NOINLINE_ATTRIBUTE
#endif  // __ICC
#endif  // NOINLINE_ATTRIBUTE

// Set attribute and return the new value

namespace viam {
namespace sdk {

class ModuleService;

enum class log_level : std::int8_t {
    trace = -2,
    debug = -1,
    info = 0,  // default value is info
    warning = 1,
    error = 2,
    fatal = 3,
};
class Logger {
   public:
    explicit Logger(std::string name);
    explicit Logger(std::string name, log_level level);

    void set_log_level(log_level level);
    void log(const std::string& msg, log_level level, const char* file, int line_no) const;
    static log_level from_string(std::string str);

    struct None {};

    template <typename List>
    struct LogData {
        List list;
    };

    template <typename List>
    void static static_log(const LogData<List>& data,
                           log_level level,
                           const char* file,
                           int line_no) {
        std::ostringstream buffer;
        output(buffer, std::move(data.list));
        static_log_(buffer.str(), level, file, line_no);
    };

    template <typename Begin, typename Value>
    friend constexpr LogData<std::pair<Begin&&, Value&&>> operator<<(LogData<Begin>&& begin,
                                                                     Value&& value) noexcept {
        return {{std::forward<Begin>(begin.list), std::forward<Value>(value)}};
    }

    template <typename Begin, size_t n>
    friend constexpr LogData<std::pair<Begin&&, const char*>> operator<<(
        LogData<Begin>&& begin, const char (&value)[n]) noexcept {
        return {{std::forward<Begin>(begin.list), value}};
    }

    typedef std::ostringstream& (*PfnManipulator)(std::ostringstream&);

    template <typename Begin>
    friend constexpr LogData<std::pair<Begin&&, PfnManipulator>> operator<<(
        LogData<Begin>&& begin, PfnManipulator value) noexcept {
        return {{std::forward<Begin>(begin.list), value}};
    }

    template <typename Begin, typename Last>
    friend void output(std::ostringstream& os, const std::pair<Begin, Last>& data) {
        output(os, std::move(data.first));
        os << data.second;
    }

    friend inline void output(std::ostringstream& os, None) {}

    template <typename List>
    void log(const LogData<List>& data, log_level level, const char* file, int line_no) const {
        std::ostringstream buffer;
        output(buffer, std::move(data.list));
        log(buffer.str(), level, file, line_no);
    }
    NOINLINE_ATTRIBUTE;

    ~Logger();

    friend class ModuleService;

    // (RSDK-9172) These (or at least the default version) should be called from within an
    // initializer
    // object that handles all SDK initialization for us.
    void static init_logging();
    void static init_logging(std::ostream& custom_strm);

   private:
    void static static_log_(const std::string& msg, log_level level, const char* file, int line_no);
    struct impl;

    std::string name_;
    log_level level_;
    std::unique_ptr<impl> impl_;
};

}  // namespace sdk
}  // namespace viam

//  New macro that includes severity, filename and line number
#define VIAM_SDK_CUSTOM_FORMATTED_LOG(logger, sev, msg) \
    logger->log(                                        \
        ::viam::sdk::Logger::LogData<::viam::sdk::Logger::None>() << msg, sev, __FILE__, __LINE__)

//  New macro that includes severity, filename and line number, using a default built-in logger
#define VIAM_SDK_TRIVIAL_CUSTOM_FORMATTED_LOG(sev, msg) \
    ::viam::sdk::Logger::static_log(                    \
        ::viam::sdk::Logger::LogData<::viam::sdk::Logger::None>() << msg, sev, __FILE__, __LINE__)
