#include "../include/logorithm/Logger.h"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <source_location>
#include <string>

using enum LOGLEVEL;

namespace {
    std::string sourceToString(const std::source_location source) {
        return std::format("{}:{}:{}",
                           std::filesystem::path(source.file_name()).filename().string(),
                           source.function_name(),
                           source.line());
    }

    std::string getFormattedDate() {
        using namespace std::chrono;
        return std::format("{:%Y-%m-%d %H:%M:%S}", floor<seconds>(system_clock::now()));
    }

    std::string logLevelToString(const LOGLEVEL logLevel) {
        switch (logLevel) {
            case Fatal:   return "Fatal";
            case Error:   return "Error";
            case Warning: return "Warning";
            case Info:    return "Info";
            case Debug:   return "Debug";
            case Trace:   return "Trace";
            default:      return "Unknown";
        }
    }
}

void Logger::setMinimumLogLevel(const LOGLEVEL logLevelFilter) {
    minLogLevel = logLevelFilter;
}

void Logger::log(const std::string_view &text, LOGLEVEL level, std::source_location const source) {
    std::scoped_lock lock(mutex);
    if (level > minLogLevel || errorReported.load() || level == None || level == All)
        return;
    file << "[" << logLevelToString(level) << "] " << getFormattedDate() << " | " << sourceToString(source) << " | " << text << std::endl;
    if (file.fail() && !errorReported.exchange(true)) {
        std::cerr << "Failed to write to log file: " << logFileName << std::endl;
    }
}
const std::filesystem::path &Logger::getLogFileName() const {
    return logFileName;
}
