/**
 * @file log_test.cpp
 * @author Florent ALBANY - FAL
 * @brief Categorized test suite for CTool logging utilities (ctool::log) using Google Test.
 * @version 1.0
 * @date 2026-05-31
 */

#include <gtest/gtest.h>
#include "src/ctool/log.hpp"
#include <sstream>
#include <thread>
#include <vector>

using namespace ctool::log;

// ------------------------------------------------------------------------
// CATEGORY: VERBOSITY LEVELS
// ------------------------------------------------------------------------

TEST(LogVerbosity, Thresholds) {
    std::stringstream ss;

    // Test Quiet: should log nothing regardless of message level
    message(Level::Error, "Should not appear", ss, Level::Quiet);
    EXPECT_TRUE(ss.str().empty());

    // Test Error level threshold
    message(Level::Error, "Error msg", ss, Level::Error);
    EXPECT_NE(ss.str().find("[ERROR]"), std::string::npos);
    ss.str("");
    ss.clear();

    message(Level::Warning, "Warning msg", ss, Level::Error);
    EXPECT_TRUE(ss.str().empty());

    // Test Info level threshold
    message(Level::Error, "Error msg", ss, Level::Info);
    EXPECT_NE(ss.str().find("[ERROR]"), std::string::npos);
    ss.str("");
    ss.clear();

    message(Level::Info, "Info msg", ss, Level::Info);
    EXPECT_NE(ss.str().find("[INFO ]"), std::string::npos);
    ss.str("");
    ss.clear();

    message(Level::Debug, "Debug msg", ss, Level::Info);
    EXPECT_TRUE(ss.str().empty());
}

// ------------------------------------------------------------------------
// CATEGORY: FORMATTING & CONTENT
// ------------------------------------------------------------------------

TEST(LogFormatting, StandardOutput) {
    std::stringstream ss;
    message(Level::Debug, "Detailed debug message", ss, Level::Debug);

    std::string output = ss.str();
    // Format: YYYY-MM-DD HH:MM:SS [LEVEL] Message\n

    EXPECT_NE(output.find("[DEBUG]"), std::string::npos);
    EXPECT_NE(output.find("Detailed debug message"), std::string::npos);
    EXPECT_EQ(output.back(), '\n');

    // Basic timestamp validation (YYYY-MM-DD)
    EXPECT_EQ(output[4], '-');
    EXPECT_EQ(output[7], '-');
    EXPECT_EQ(output[10], ' ');
    EXPECT_EQ(output[13], ':');
    EXPECT_EQ(output[16], ':');
}

TEST(LogFormatting, LevelStrings) {
    std::stringstream ss;

    message(Level::Error, "Msg", ss);
    EXPECT_NE(ss.str().find("[ERROR]"), std::string::npos);
    ss.str("");
    ss.clear();

    message(Level::Warning, "Msg", ss);
    EXPECT_NE(ss.str().find("[WARN ]"), std::string::npos);
    ss.str("");
    ss.clear();

    message(Level::Info, "Msg", ss);
    EXPECT_NE(ss.str().find("[INFO ]"), std::string::npos);
    ss.str("");
    ss.clear();

    message(Level::Debug, "Msg", ss);
    EXPECT_NE(ss.str().find("[DEBUG]"), std::string::npos);
    ss.str("");
    ss.clear();

    // Default case (if somehow passed an invalid enum value)
    message(static_cast<Level>(99), "Unknown", ss, static_cast<Level>(100));
    EXPECT_NE(ss.str().find("[LOG  ]"), std::string::npos);
}

// ------------------------------------------------------------------------
// CATEGORY: CONCURRENCY
// ------------------------------------------------------------------------

TEST(LogConcurrency, ThreadSafety) {
    std::stringstream        ss;
    const int                numThreads    = 10;
    const int                msgsPerThread = 50;
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&ss, i, msgsPerThread]() {
            for (int j = 0; j < msgsPerThread; ++j) {
                message(Level::Info, "Thread " + std::to_string(i) + " msg " + std::to_string(j), ss);
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // Verify total line count matches expected number of messages
    std::string content   = ss.str();
    int         lineCount = 0;
    size_t      pos       = 0;
    while ((pos = content.find('\n', pos)) != std::string::npos) {
        lineCount++;
        pos++;
    }
    EXPECT_EQ(lineCount, numThreads * msgsPerThread);
}

// ------------------------------------------------------------------------
// CATEGORY: WRAPPERS (STDOUT/STDERR)
// ------------------------------------------------------------------------

TEST(LogWrappers, StdOutErr) {
    // Calling these primarily to ensure no crashes/deadlocks
    EXPECT_NO_THROW(toCout(Level::Info, "Testing stdout wrapper", Level::Info));
    EXPECT_NO_THROW(toCerr(Level::Error, "Testing stderr wrapper", Level::Error));

    // Verify silent behavior
    EXPECT_NO_THROW(toCout(Level::Debug, "Should be silent", Level::Info));
}
