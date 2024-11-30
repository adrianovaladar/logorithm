#ifndef KAGEBUNSHIN_LOGGERTESTS_H
#define KAGEBUNSHIN_LOGGERTESTS_H

#include "../include/logorithm/Logger.h"
#include <fstream>
#include <gtest/gtest.h>

// Define a test fixture class
class LoggerTests : public ::testing::Test {
public:
    LoggerTests() = default;

protected:
    std::ifstream logFile;
    Logger &loggerTests = Logger::getInstance();
    // Set up common data or objects before each test case
    void SetUp() override {
        // Initialize any resources needed for testing
        logFile.open(loggerTests.getLogFileName());
        if (!logFile.is_open()) {
            // Handle error: Failed to open log file
            // You might want to fail the test or log a message
            FAIL() << "Failed to open log file: " << loggerTests.getLogFileName();
        }
    }

    // Clean up after each test case
    void TearDown() override {
        // Release any resources allocated during testing
        logFile.close();
    }

};


#endif//KAGEBUNSHIN_LOGGERTESTS_H
