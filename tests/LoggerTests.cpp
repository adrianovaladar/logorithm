#include "LoggerTests.h"
#include <fstream>
#include <thread>

using enum LOGLEVEL;

TEST_F(LoggerTests, CheckLogsAllLevels) {
    logger.log<Trace>("This is a trace message.");
    logger.log<Debug>("This is a debug message.");
    logger.log<Info>("This is an info message.");
    logger.log<Warning>("This is a warning message.");
    logger.log<Error>("This is an error message.");
    logger.log<Fatal>("This is a fatal message.");

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
    loggerTests.log<Info>("Test message");

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
        loggerTests.log<Info>("Test multithreading message");
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
    loggerTests.log<Info>("Info message");
    loggerTests.log<Error>("Error message");

    storeLogs();

    std::stringstream buffer;
    buffer << logFile.rdbuf();
    std::string logContents = buffer.str();

    ASSERT_EQ(std::string::npos, logContents.find("Info message"));
    ASSERT_NE(std::string::npos, logContents.find("Error message"));
}
