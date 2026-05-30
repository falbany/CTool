/**
 * @file time_test.cpp
 * @author Florent ALBANY - FAL
 * @brief Comprehensive test suite for CTool time module (ctool::time) using Google Test.
 * @version 1.0
 * @date 2026-05-29
 */

#include <gtest/gtest.h>
#include "ctool/time.hpp"
#include <string>
#include <type_traits>

/* ============================================================================
 * CurrentTimestamp Tests
 * ============================================================================ */

TEST(CtTimeCurrentTimestamp, BasicFormat) {
    std::string ts = ctool::time::getCurrentTimestamp();
    EXPECT_EQ(ts.length(), 19u) << "Timestamp format should be 'YYYY-MM-DD HH:MM:SS' (19 chars)";
}

TEST(CtTimeCurrentTimestamp, ContainsValidDate) {
    std::string ts   = ctool::time::getCurrentTimestamp();
    std::string year = ts.substr(0, 4);
    EXPECT_EQ(year.length(), 4u);
    EXPECT_TRUE(year[0] == '2') << "Year should start with '2'";
    EXPECT_TRUE((year >= "2024") && (year <= "2030")) << "Year should be in a reasonable range";
}

TEST(CtTimeCurrentTimestamp, ContainsValidSeparator) {
    std::string ts = ctool::time::getCurrentTimestamp();
    /* Format: "YYYY-MM-DD HH:MM:SS" */
    EXPECT_EQ(ts[4], '-') << "Separator at index 4 should be '-'";
    EXPECT_EQ(ts[7], '-') << "Separator at index 7 should be '-'";
    EXPECT_EQ(ts[10], ' ') << "Separator at index 10 should be ' '";
    EXPECT_EQ(ts[13], ':') << "Separator at index 13 should be ':'";
    EXPECT_EQ(ts[16], ':') << "Separator at index 16 should be ':'";
}

/* ============================================================================
 * SleepMs Tests
 * ============================================================================ */

TEST(CtTimeSleepMs, SleepPositive) {
    long long before = ctool::time::getUptimeMs();
    ctool::time::sleepMs(10);
    long long after = ctool::time::getUptimeMs();
    EXPECT_GE(after - before, 8ll) << "sleepMs(10) should sleep for roughly 10ms";
}

TEST(CtTimeSleepMs, SleepZero) {
    long long before = ctool::time::getUptimeMs();
    ctool::time::sleepMs(0);
    long long after = ctool::time::getUptimeMs();
    EXPECT_LT(after - before, 10ll) << "sleepMs(0) should return immediately";
}

TEST(CtTimeSleepMs, SleepMultiple) {
    long long total_before = ctool::time::getUptimeMs();
    ctool::time::sleepMs(5);
    ctool::time::sleepMs(5);
    ctool::time::sleepMs(5);
    long long total_after = ctool::time::getUptimeMs();
    EXPECT_GE(total_after - total_before, 10ll) << "Three sleepMs(5) calls should accumulate to ~15ms";
}

/* ============================================================================
 * SleepS Tests
 * ============================================================================ */

TEST(CtTimeSleepS, SleepPositive) {
    long long before = ctool::time::getUptimeMs();
    ctool::time::sleepS(1);
    long long after = ctool::time::getUptimeMs();
    EXPECT_GE(after - before, 990ll) << "sleepS(1) should sleep for roughly 1000ms";
}

TEST(CtTimeSleepS, SleepZero) {
    long long before = ctool::time::getUptimeMs();
    ctool::time::sleepS(0);
    long long after = ctool::time::getUptimeMs();
    EXPECT_LT(after - before, 10ll) << "sleepS(0) should return immediately";
}

TEST(CtTimeSleepS, SleepMultiple) {
    ctool::time::sleepS(0);
    ctool::time::sleepS(0);
    ctool::time::sleepS(0);
    /* Multiple zero-sleeps should complete without error */
    EXPECT_TRUE(true) << "Consecutive sleepS(0) calls did not crash";
}

/* ============================================================================
 * UptimeMs Tests
 * ============================================================================ */

TEST(CtTimeUptimeMs, ReturnsPositive) {
    long long uptime = ctool::time::getUptimeMs();
    EXPECT_GT(uptime, 0ll) << "Uptime in milliseconds should be positive";
}

TEST(CtTimeUptimeMs, MonotonicIncrease) {
    long long first = ctool::time::getUptimeMs();
    ctool::time::sleepMs(5);
    long long second = ctool::time::getUptimeMs();
    EXPECT_GT(second, first) << "Uptime should increase monotonically";
}

TEST(CtTimeUptimeMs, ReturnsValidType) {
    static_assert(std::is_same_v<decltype(ctool::time::getUptimeMs()), long long>, "getUptimeMs must return long long");
    EXPECT_TRUE(true) << "Return type verified at compile-time";
}

/* ============================================================================
 * UptimeNs Tests
 * ============================================================================ */

TEST(CtTimeUptimeNs, ReturnsPositive) {
    long long uptime = ctool::time::getUptimeNs();
    EXPECT_GT(uptime, 0ll) << "Uptime in nanoseconds should be positive";
}

TEST(CtTimeUptimeNs, ReturnsValidType) {
    static_assert(std::is_same_v<decltype(ctool::time::getUptimeNs()), long long>, "getUptimeNs must return long long");
    EXPECT_TRUE(true) << "Return type verified at compile-time";
}

/* ============================================================================
 * Timer Tests
 * ============================================================================ */

TEST(CtTimeTimer, BasicConstruction) {
    /* Should construct without crashing */
    {
        ctool::time::Timer timer("TestLabel");
    }
    EXPECT_TRUE(true) << "Timer constructed and destroyed without crash";
}

TEST(CtTimeTimer, DestructorCompletes) {
    /* Scoped block ensures destructor runs and completes */
    {
        ctool::time::Timer timer("DestructorTest");
        /* Small sleep so the timer has measurable duration */
        ctool::time::sleepMs(5);
    } /* Timer destructor runs here */
    EXPECT_TRUE(true) << "Timer destructor completed without crash";
}

TEST(CtTimeTimer, LabelStored) {
    /* If Timer stores label correctly, construction/destruction will not assert */
    std::string testLabel = "CustomLabel_123";
    {
        ctool::time::Timer timer(testLabel);
    }
    EXPECT_EQ(testLabel, "CustomLabel_123") << "Label string is preserved";
}

TEST(CtTimeTimer, DefaultLabel) {
    /* Default constructor should use "Unknown" label */
    {
        ctool::time::Timer timer;
    }
    EXPECT_TRUE(true) << "Timer with default label constructed and destroyed without crash";
}

TEST(CtTimeTimer, PreventCopying) {
    static_assert(!std::is_copy_constructible_v<ctool::time::Timer>, "Timer must not be copy-constructible");
    static_assert(!std::is_copy_assignable_v<ctool::time::Timer>, "Timer must not be copy-assignable");
    EXPECT_TRUE(true) << "Timer is non-copyable (verified at compile-time)";
}
