#pragma once

#include <memory>
#include <spdlog/spdlog.h>

enum class LogLevel {
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warn = 3,
    Error = 4,
    Critical = 5,
    Off = 6
};

class Logger {
public:
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
    ~Logger() = default;

    static Logger &instance();
    void set_level(LogLevel level);
    void flush();

    template<typename... Args>
    void trace(const char *filename, int line, const char *func, spdlog::format_string_t<Args...> fmt, Args &&...args);
    template<typename... Args>
    void debug(const char *filename, int line, const char *func, spdlog::format_string_t<Args...> fmt, Args &&...args);
    template<typename... Args>
    void info(const char *filename, int line, const char *func, spdlog::format_string_t<Args...> fmt, Args &&...args);
    template<typename... Args>
    void warn(const char *filename, int line, const char *func, spdlog::format_string_t<Args...> fmt, Args &&...args);
    template<typename... Args>
    void error(const char *filename, int line, const char *func, spdlog::format_string_t<Args...> fmt, Args &&...args);
    template<typename... Args>
    void critical(const char *filename, int line, const char *func, spdlog::format_string_t<Args...> fmt, Args &&...args);

private:
    Logger();

    std::shared_ptr<spdlog::logger> logger_ = nullptr;
};
template<typename... Args>
void Logger::trace(const char *filename, int line, const char *func, spdlog::format_string_t<Args...> fmt, Args &&...args)
{
    if (logger_ == nullptr) {
        throw std::runtime_error("logger_ is null");
    }
    logger_->log(spdlog::source_loc{filename, line, func}, spdlog::level::trace, fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void Logger::debug(const char *filename, int line, const char *func, spdlog::format_string_t<Args...> fmt, Args &&...args)
{
    if (logger_ == nullptr) {
        throw std::runtime_error("logger_ is null");
    }
    logger_->log(spdlog::source_loc{filename, line, func}, spdlog::level::debug, fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void Logger::info(const char *filename, int line, const char *func, spdlog::format_string_t<Args...> fmt, Args &&...args)
{
    if (logger_ == nullptr) {
        throw std::runtime_error("logger_ is null");
    }
    logger_->log(spdlog::source_loc{filename, line, func}, spdlog::level::info, fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void Logger::warn(const char *filename, int line, const char *func, spdlog::format_string_t<Args...> fmt, Args &&...args)
{
    if (logger_ == nullptr) {
        throw std::runtime_error("logger_ is null");
    }
    logger_->log(spdlog::source_loc{filename, line, func}, spdlog::level::warn, fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void Logger::error(const char *filename, int line, const char *func, spdlog::format_string_t<Args...> fmt, Args &&...args)
{
    if (logger_ == nullptr) {
        throw std::runtime_error("logger_ is null");
    }
    logger_->log(spdlog::source_loc{filename, line, func}, spdlog::level::err, fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void Logger::critical(const char *filename, int line, const char *func, spdlog::format_string_t<Args...> fmt, Args &&...args)
{
    if (logger_ == nullptr) {
        throw std::runtime_error("logger_ is null");
    }
    logger_->log(spdlog::source_loc{filename, line, func}, spdlog::level::critical, fmt, std::forward<Args>(args)...);
}

#define LOG_TRACE(...) Logger::instance().trace(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_DEBUG(...) Logger::instance().debug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_INFO(...) Logger::instance().info(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_WARN(...) Logger::instance().warn(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_ERROR(...) Logger::instance().error(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_CRITICAL(...) Logger::instance().critical(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_FLUSH() Logger::instance().flush()