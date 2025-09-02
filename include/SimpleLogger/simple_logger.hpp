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
        virtual bool is_active() const { return true; }

        virtual ~SimpleLogger() = default;
    };

    class NullLogger : public SimpleLogger {
        virtual LogLevel get_log_level() const override { return LogLevel::Info; }
        virtual void set_log_level(LogLevel level) override {}
        void log(LogLevel level, std::string_view message) override {}
        bool is_active() const override { return false; }
    };

    inline NullLogger* null_logger() {
        static NullLogger instance;
        return &instance;
    }

    inline SimpleLogger* normalize(SimpleLogger* logger) {
        return logger ? logger : null_logger();
    }
} // namespace simple_logger
