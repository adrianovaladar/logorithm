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

void Logger::log(const std::string_view &text, const LOGLEVEL level,
                 const std::vector<std::pair<std::string, std::string>> &fields, const std::source_location source) {
    if (level > minLogLevel || errorReported.load() || level == None || level == All)
        return;
    std::string log =
            std::format("[{}] {} | {} | {}", logLevelToString(level), getFormattedDate(), sourceToString(source), text);
    for (const auto &[key, value]: fields) {
        log += std::format(" | {} = {}", key, value);
    }
    std::scoped_lock lock(mutex);
    file << log << std::endl; // std::endl flushes the output buffer
    if (file.fail() && !errorReported.exchange(true)) {
        std::cerr << "Failed to write to log file: " << logFileName << std::endl;
    }
}

const std::filesystem::path &Logger::getLogFileName() const {
    return logFileName;
}

void Logger::fillLogFileName(const std::filesystem::path& directoryName) {
    using namespace std::chrono;
    logFileName = directoryName / std::format("log_{:%Y-%m-%d}", system_clock::now());
    for (int i = 0; i < maximumFileCount; ++i) {
        const std::filesystem::path fileNameToSearch = logFileName.parent_path() /
            std::format("{}_{:03}.txt", logFileName.stem().string(), i);
        if (!exists(fileNameToSearch) ||
            (exists(fileNameToSearch) && file_size(fileNameToSearch) < maximumSize)) {
            logFileName = fileNameToSearch;
            return;
            }
    }
    logFileName += ".txt";
}
