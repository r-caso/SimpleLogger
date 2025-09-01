#pragma once

#include <string_view>

namespace iif_sadaf::talk::simple_logger {

enum class LogLevel {
    Info,
    Debug,
    Trace,
};

class SimpleLogger {
public:
    virtual LogLevel get_log_level() const = 0;
    virtual void set_log_level(LogLevel level) = 0;

    virtual void log(LogLevel level, std::string_view message) = 0;

    void info(std::string_view message) { log(LogLevel::Info, message); }
    void debug(std::string_view message) { log(LogLevel::Debug, message); }
    void trace(std::string_view message) { log(LogLevel::Trace, message); }

    virtual ~SimpleLogger() = default;
};

} // namespace simple_logger
