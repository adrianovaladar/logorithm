#ifndef LOGORITHM_LOGGER_H
#define LOGORITHM_LOGGER_H

#include <atomic>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <source_location>
#include <string>
#include <chrono>
/**
 * @brief Enumeration for log levels.
 */
enum class LOGLEVEL : int {
    None = 0,    /**< No logging at all. */
    Fatal = 1,   /**< Fatal message. */
    Error = 2,   /**< Error message. */
    Warning = 3, /**< Warning message. */
    Info = 4,    /**< Informational message. */
    Debug = 5,   /**< Debug message. */
    Trace = 6,   /**< Trace message. */
    All = 7      /**< Log everything. */
};

/**
 * @brief Meyer’s Singleton class for logging messages.
 */
class Logger final {
public:
    /**
     * @brief Get the singleton instance of Logger.
     * @return Reference to the Logger instance.
     */
    static Logger &getInstance() {
        static Logger instance;// Static local variable ensures lazy initialization and thread safety
        return instance;
    }
    Logger(const Logger &) = delete;           // Disable copy constructor
    Logger &operator=(const Logger &) = delete;// Disable copy assignment operator
    /**
     * @brief Log a message.
     * @param text The text of the log message.
     * @param level The log level.
     * @param fields Optional pairs of strings to include additional information in the log.
     * @param source The source location of the log message.
     */
    void log(const std::string_view &text, LOGLEVEL level,
             const std::vector<std::pair<std::string, std::string>> &fields = {},
             std::source_location source = std::source_location::current());
    /**
     * @brief Get the file name of the log file.
     * @return The file name of the log file.
     */
    const std::filesystem::path &getLogFileName() const;
    /**
     * @brief Sets the minimum log level to be considered for logging.
     *
     * This method updates the minimum log level, filtering out any log messages
     * that are below the specified level. By default, the logger considers all levels.
     *
     * @param logLevelFilter The new minimum log level to be set.
     */
    void setMinimumLogLevel(LOGLEVEL logLevelFilter);
private:
    std::mutex mutex {};/**< Mutex for thread safety. */
    std::filesystem::path logFileName;/**< File name of the log file. */
    std::atomic<bool> errorReported{false};/**< Atomic boolean flag indicating whether an error has been reported. */
    std::ofstream file;/**< Output file stream for logging. */
    std::atomic<LOGLEVEL> minLogLevel{LOGLEVEL::All}; /**< Minimum log level to be considered. Default to All */
    size_t maximumSize{10 * 1024 * 1024};
    void fillLogFileName(const std::filesystem::path& directoryName);
    /**
     * @brief Private constructor to prevent instantiation from outside.
     */
    Logger() {
        const std::filesystem::path directoryName = "logs";
        if (std::error_code ec{}; !exists(directoryName) && !is_directory(directoryName) && !create_directory(directoryName, ec)) {
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
     * @brief Destructor to close the log file.
     */
    ~Logger() = default;
};
/**
 * @brief Global logger instance.
 *
 * This global variable provides access to the singleton instance of the Logger class.
 * It can be used throughout the code.
 */
inline Logger &logger = Logger::getInstance();


#endif//LOGORITHM_LOGGER_H
