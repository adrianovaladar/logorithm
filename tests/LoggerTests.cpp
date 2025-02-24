#include "LoggerTests.h"
#include <fstream>
#include <thread>

using enum LOGLEVEL;

TEST_F(LoggerTests, CheckLogsAllLevels) {
    logger.log("This is a trace message.", Trace);
    logger.log("This is a debug message.", Debug);
    logger.log("This is an info message.", Info);
    logger.log("This is a warning message.", Warning);
    logger.log("This is an error message.", Error);
    logger.log("This is a fatal message.", Fatal);

    storeLogs();

    std::stringstream buffer;
    buffer << logFile.rdbuf();
    std::string logContents = buffer.str();

    ASSERT_NE(std::string::npos, logContents.find("trace message."));
    ASSERT_NE(std::string::npos, logContents.find("debug message."));
    ASSERT_NE(std::string::npos, logContents.find("info message."));
    ASSERT_NE(std::string::npos, logContents.find("warning message."));
    ASSERT_NE(std::string::npos, logContents.find("error message."));
    ASSERT_NE(std::string::npos, logContents.find("fatal message."));
}

TEST_F(LoggerTests, LogMessageTest) {
    loggerTests.log("Test message", Info);

    storeLogs();

    std::stringstream buffer;
    buffer << logFile.rdbuf();
    std::string logContents = buffer.str();

    ASSERT_NE(std::string::npos, logContents.find("Test message"));
}

TEST_F(LoggerTests, MultipleLogMessagesTest) {
    constexpr int numberThreads{100};
    std::array<std::jthread, numberThreads> threads;
    auto logMessage = [&]() {
        loggerTests.log("Test multithreading message", Info);
    };
    for (int i{}; i < threads.size(); i++)
        threads.at(i) = std::jthread(logMessage);
    for (int i{}; i < threads.size(); i++)
        threads.at(i).join();

    storeLogs();

    std::stringstream buffer;
    buffer << logFile.rdbuf();
    std::string logContents = buffer.str();

    for (int i{}; i < threads.size(); i++)
        ASSERT_NE(std::string::npos, logContents.find("Test multithreading message"));
}

TEST_F(LoggerTests, LogOnlyError) {
    logger.setMinimumLogLevel(Error);
    loggerTests.log("Info message", Info);
    loggerTests.log("Error message", Error);

    storeLogs();

    std::stringstream buffer;
    buffer << logFile.rdbuf();
    std::string logContents = buffer.str();

    ASSERT_EQ(std::string::npos, logContents.find("Info message"));
    ASSERT_NE(std::string::npos, logContents.find("Error message"));
}
