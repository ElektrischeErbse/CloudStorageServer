#include <cstdarg>
#include <logger.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

Logger &Logger::instance()
{
    static Logger logger;
    return logger;
}

void Logger::set_level(LogLevel level)
{
    if (logger_ == nullptr) {
        throw std::runtime_error("logger_ is null");
    }
    logger_->set_level(static_cast<spdlog::level::level_enum>(level));
}


void Logger::flush()
{
    if (logger_ == nullptr) {
        throw std::runtime_error("logger_ is null");
    }
    logger_->flush();
}

Logger::Logger()
{
    std::vector<spdlog::sink_ptr> sinks;
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%# %!] %v");
    sinks.push_back(console_sink);

    auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("server.log", 0, 0, true, 30);
    file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%# %!] %v");
    sinks.push_back(file_sink);

    logger_ = std::make_shared<spdlog::logger>("global_logger", sinks.begin(), sinks.end());
    logger_->set_level(static_cast<spdlog::level::level_enum>(LogLevel::Debug));
    logger_->flush_on(spdlog::level::err);
    spdlog::register_logger(logger_);
    // logger_->log(spdlog::source_loc{__FILE__, __LINE__, __FUNCTION__}, spdlog::level::debug, "Log registration successful");
}