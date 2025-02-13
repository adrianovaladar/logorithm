#include "LoggerTests.h"
#include <fstream>

using enum LOGLEVEL;

TEST_F(LoggerTests, LogMessageTest) {
    loggerTests.log("Test message", Info);

    std::stringstream buffer;
    buffer << logFile.rdbuf();
    std::string logContents = buffer.str();

    ASSERT_NE(std::string::npos, logContents.find("Test message"));
}

TEST_F(LoggerTests, MultipleLogMessagesTest) {
    constexpr int numberThreads{100};
    std::array<std::thread, numberThreads> threads;
    auto logMessage = [&]() {
        loggerTests.log("Test multithreading message", Info);
    };
    for (int i{}; i < threads.size(); i++)
        threads.at(i) = std::thread(logMessage);
    for (int i{}; i < threads.size(); i++)
        threads.at(i).join();

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

    std::stringstream buffer;
    buffer << logFile.rdbuf();
    std::string logContents = buffer.str();
    ASSERT_EQ(std::string::npos, logContents.find("Info message"));
    ASSERT_NE(std::string::npos, logContents.find("Error message"));
}
