#ifndef LOGORITHM_LOGGER_H
#define LOGORITHM_LOGGER_H

#include <source_location>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>

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
     * @brief Sets the minimum log level to be considered for logging.
     *
     * This method updates the minimum log level, filtering out any log messages
     * that are below the specified level. By default, the logger considers all levels.
     *
     * @param logLevelFilter The new minimum log level to be set.
     */
    void setMinimumLogLevel(LOGLEVEL logLevelFilter);
    /**
     * @brief Get the file name of the log file.
     * @return The file name of the log file.
     */
    [[nodiscard]] const std::filesystem::path &getLogFileName() const;
private:
    class LoggerPImpl;
    std::unique_ptr<LoggerPImpl> impl;
    /**
     * @brief Private constructor to prevent instantiation from outside.
     */
    Logger();
    /**
     * @brief Destructor to close the log file.
     */
    ~Logger();
};
/**
 * @brief Global logger instance.
 *
 * This global variable provides access to the singleton instance of the Logger class.
 * It can be used throughout the code.
 */
inline Logger &logger = Logger::getInstance();


#endif//LOGORITHM_LOGGER_H
