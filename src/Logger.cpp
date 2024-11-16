#include "logorithm/Logger.h"
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <source_location>
#include <string>

std::string sourceToString(std::source_location const source) {
    std::stringstream ss;
    ss << std::filesystem::path(source.file_name()).filename().string() << ":" << source.function_name() << ":" << source.line();
    return ss.str();
}

std::string getFormattedDate() {
    const auto now = std::chrono::system_clock::now();
    const auto nowTime = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm{};
    localtime_r(&nowTime, &now_tm);
    std::stringstream ss;
    ss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void Logger::log(const std::string &text, LOGLEVEL level, std::source_location const source) {
    std::scoped_lock lock(mutex);
    if (errorReported.load())
        return;
    file << "[" << static_cast<char>(level) << "] " << getFormattedDate() << " | " << sourceToString(source) << " | " << text << std::endl;
    if (file.fail() && !errorReported.exchange(true)) {
        std::cerr << "Failed to write to log file: " << logFileName << std::endl;
    }
}
const std::filesystem::path &Logger::getLogFileName() const {
    return logFileName;
}
