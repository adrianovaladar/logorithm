#include "LoggerTests.h"
#include <fstream>

TEST_F(LoggerTests, LogMessageTest) {
    loggerTests.log("Test message", LOGLEVEL::Info);

    std::stringstream buffer;
    buffer << logFile.rdbuf();
    std::string logContents = buffer.str();

    ASSERT_NE(std::string::npos, logContents.find("Test message"));
}

TEST_F(LoggerTests, MultipleLogMessagesTest) {
    constexpr int numberThreads{100};
    std::array<std::thread, numberThreads> threads;
    auto logMessage = [&]() {
        loggerTests.log("Test multithreading message", LOGLEVEL::Info);
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
