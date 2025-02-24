#include "../include/logorithm/Logger.h"
#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <source_location>
#include <string>

using enum LOGLEVEL;

namespace {
    /**
     * @brief Converts a source location to a string representation.
     * @param source The source location.
     * @return A formatted string containing file name, function name and line number.
     */
    std::string sourceToString(const std::source_location source) {
        return std::format("{}:{}:{}",
                           std::filesystem::path(source.file_name()).filename().string(),
                           source.function_name(),
                           source.line());
    }
    /**
     * @brief Retrieves the current timestamp formatted as a string.
     * @return A string representing the current date and time in YYYY-MM-DD HH:MM:SS format.
     */
    std::string getFormattedDate() {
        using namespace std::chrono;
        return std::format("{:%Y-%m-%d %H:%M:%S}", floor<seconds>(system_clock::now()));
    }
    /**
     * @brief Converts a LOGLEVEL enumeration to a string representation.
     * @param logLevel The log level.
     * @return A string representing the log level.
     */
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
/**
 * @brief Private implementation of the Logger class.
 */
class Logger::LoggerPImpl {
    std::mutex mutex{}; /**< Mutex for thread safety. */
    std::filesystem::path logFileName; /**< File name of the log file. */
    std::atomic<bool> errorReported{false}; /**< Atomic boolean flag indicating whether an error has been reported. */
    std::ofstream file; /**< Output file stream for logging. */
    std::atomic<LOGLEVEL> minLogLevel{LOGLEVEL::All}; /**< Minimum log level to be considered. Default to All */
    const size_t maximumSize{10 * 1024 * 1024}; /**< Maximum allowed log file size in bytes (10 MB). */
    const int maximumFileCount = 1000; /**< Maximum number of log files to attempt before stopping. */
    std::vector<std::string> logsBuffer; /**< Buffer to store logs before writing to the file. */
    const size_t bufferSize = 100; /**< Maximum number of log messages before flushing to the file. */
public:
    /**
     * @brief Constructs the LoggerPImpl instance.
     * Initializes the log directory and opens the log file.
     */
    LoggerPImpl() {
        const std::filesystem::path directoryName = "logs";
        if (std::error_code ec{};
            !exists(directoryName) && !is_directory(directoryName) && !create_directory(directoryName, ec)) {
            std::cerr << "Failed to create log directory: " << ec.message() << std::endl;
            errorReported.exchange(true);
        }
        fillLogFileName(directoryName);
        file.open(logFileName, std::ofstream::app);
        if (!file.is_open()) {
            std::cerr << "Failed to open log file: " << logFileName << std::endl;
            errorReported.exchange(true);
        }
    }
    /**
     * @brief Destructor for LoggerPImpl.
     */
    ~LoggerPImpl() { storeLogs(); }
    /**
     * @brief Determines the appropriate log file name based on date and size.
     * @param directoryName The directory where logs are stored.
     */
    void fillLogFileName(const std::filesystem::path &directoryName) {
        using namespace std::chrono;
        logFileName = directoryName / std::format("log_{:%Y-%m-%d}", system_clock::now());
        for (int i = 0; i < maximumFileCount; ++i) {
            const std::filesystem::path fileNameToSearch =
                    logFileName.parent_path() / std::format("{}_{:03}.txt", logFileName.stem().string(), i);
            if (!exists(fileNameToSearch) || (exists(fileNameToSearch) && file_size(fileNameToSearch) < maximumSize)) {
                logFileName = fileNameToSearch;
                return;
            }
        }
        logFileName += ".txt";
    }
    /**
     * @brief Log a message.
     * @param text The text of the log message.
     * @param level The log level.
     * @param fields Optional pairs of strings to include additional information in the log.
     * @param source The source location of the log message.
     */
    void log(const std::string_view &text, const LOGLEVEL level,
             const std::vector<std::pair<std::string, std::string>> &fields, const std::source_location source) {
        if (level > minLogLevel || errorReported.load() || level == None || level == All)
            return;
        std::string log = std::format("[{}] {} | {} | {}", logLevelToString(level), getFormattedDate(),
                                      sourceToString(source), text);
        for (const auto &[key, value]: fields) {
            log += std::format(" | {} = {}", key, value);
        }
        {
            std::scoped_lock lock(mutex);
            logsBuffer.push_back(log);
        }
        if (logsBuffer.size() >= bufferSize) {
            storeLogs();
        }
    }
    /**
     * @brief Writes all buffered log messages to the file and clears the buffer.
     *
     * This function ensures logs are written efficiently by reducing the number
     * of disk I/O operations. It acquires a lock to prevent race conditions and
     * writes all stored messages to the file at once.
     */
    void storeLogs() {
        std::scoped_lock lock(mutex);
        for (const auto &log: logsBuffer) {
            file << log << std::endl;
        }
        file.flush();
        logsBuffer.clear();
        if (file.fail() && !errorReported.exchange(true)) {
            std::cerr << "Failed to write to log file: " << logFileName << std::endl;
        }
    }
    /**
     * @brief Sets the minimum log level to be considered for logging.
     *
     * This method updates the minimum log level, filtering out any log messages
     * that are below the specified level. By default, the logger considers all levels.
     *
     * @param logLevelFilter The new minimum log level to be set.
     */
    void setMinimumLogLevel(const LOGLEVEL logLevelFilter) { minLogLevel = logLevelFilter; }
    /**
     * @brief Get the file name of the log file.
     * @return The file name of the log file.
     */
    const std::filesystem::path &getLogFileName() const { return logFileName; }
};

Logger::~Logger() = default;

Logger::Logger() : impl{std::make_unique<LoggerPImpl>()} {}

void Logger::log(const std::string_view &text, const LOGLEVEL level,
                 const std::vector<std::pair<std::string, std::string>> &fields, const std::source_location source) {
    if (impl) {
        impl->log(text, level, fields, source);
    }
}

void Logger::setMinimumLogLevel(const LOGLEVEL logLevelFilter) {
    if (impl) {
        impl->setMinimumLogLevel(logLevelFilter);
    }
}

void Logger::storeLogs() {
    if (impl) {
        impl->storeLogs();
    }
}

const std::filesystem::path &Logger::getLogFileName() const {
    static const std::filesystem::path emptyPath;
    const std::filesystem::path &result = impl ? impl->getLogFileName() : emptyPath;
    return result;
}
