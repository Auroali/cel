#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <filesystem>

namespace cel {
    inline std::shared_ptr<spdlog::logger> stdout_file_mt(const std::string& name, const std::filesystem::path& fpath) {
        std::vector<spdlog::sink_ptr> sinks = {std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>(), std::make_shared<spdlog::sinks::basic_file_sink_mt>(fpath)};
        std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>(name, begin(sinks), end(sinks));
        logger->info("Start of log");
        return logger;
    }
    inline const auto logger = stdout_file_mt("cel_logger", "logs/cel.log");
}

#define LOG_ERROR(x) cel::logger->error(x)
#define LOG_WARN(x) cel::logger->warn(x)
#define LOG_INFO(x) cel::logger->info(x)
#define LOG_CRITICAL(x) cel::logger->critical(x)