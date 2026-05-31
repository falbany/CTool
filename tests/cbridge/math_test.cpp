/**
 * @file math_test.cpp
 * @author Florent ALBANY - FAL
 * @brief Comprehensive test suite for CBridge math module using Google Test.
 * @version 1.0
 * @date 2026-05-31
 */

#include <gtest/gtest.h>
extern "C" {
#include "CBridge.h"
#include <math.h>
}

/**
 * @namespace CBridgeMathRegression
 * @brief Tests for linear and log-linear regression functions.
 */
TEST(CBridgeMathRegression, LinearFit) {
    double x[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double y[] = {2.0, 4.0, 6.0, 8.0, 10.0};
    size_t n   = 5;

    cbridge_reg_result_t res = CBridge.math->linearFit(x, y, n);

    EXPECT_NEAR(res.slope, 2.0, 1e-9);
    EXPECT_NEAR(res.intercept, 0.0, 1e-9);
    EXPECT_NEAR(res.r_squared, 1.0, 1e-9);
}

TEST(CBridgeMathRegression, LogLinearFit) {
    double x[] = {1.0, 2.0, 3.0};
    double y[] = {exp(1.0), exp(2.0), exp(3.0)};
    size_t n   = 3;

    cbridge_reg_result_t res = CBridge.math->logLinearFit(x, y, n);

    EXPECT_NEAR(res.slope, 1.0, 1e-9);
    EXPECT_NEAR(res.intercept, 0.0, 1e-9);
    EXPECT_NEAR(res.r_squared, 1.0, 1e-9);
}

TEST(CBridgeMathRegression, LogLinearFitInvalid) {
    double x[] = {1.0, 2.0, 3.0};
    double y[] = {1.0, 0.0, 3.0}; // Non-positive y
    size_t n   = 3;

    cbridge_reg_result_t res = CBridge.math->logLinearFit(x, y, n);

    EXPECT_DOUBLE_EQ(res.slope, 0.0);
    EXPECT_DOUBLE_EQ(res.intercept, 0.0);
    EXPECT_DOUBLE_EQ(res.r_squared, 0.0);
}

TEST(CBridgeMathRegression, Residuals) {
    double x[] = {1.0, 2.0, 3.0};
    double y[] = {2.1, 3.9, 6.2};
    size_t n   = 3;
    double slope = 2.0;
    double intercept = 0.0;
    double res[3];

    CBridge.math->residuals(x, y, n, slope, intercept, res);

    EXPECT_NEAR(res[0], 0.1, 1e-9);
    EXPECT_NEAR(res[1], -0.1, 1e-9);
    EXPECT_NEAR(res[2], 0.2, 1e-9);
}

/**
 * @namespace CBridgeMathStats
 * @brief Tests for statistical functions (min, max, mean, variance, etc.).
 */
TEST(CBridgeMathStats, MinMax) {
    double data[] = {10.5, -2.0, 15.0, 0.0, 7.5};
    size_t n      = 5;

    EXPECT_DOUBLE_EQ(CBridge.math->min(data, n), -2.0);
    EXPECT_DOUBLE_EQ(CBridge.math->max(data, n), 15.0);
}

TEST(CBridgeMathStats, EmptyData) {
    EXPECT_DOUBLE_EQ(CBridge.math->min(NULL, 0), 0.0);
    EXPECT_DOUBLE_EQ(CBridge.math->max(NULL, 0), 0.0);
    EXPECT_DOUBLE_EQ(CBridge.math->mean(NULL, 0), 0.0);
    EXPECT_DOUBLE_EQ(CBridge.math->variance(NULL, 0), 0.0);
    EXPECT_DOUBLE_EQ(CBridge.math->standardDeviation(NULL, 0), 0.0);
}

TEST(CBridgeMathStats, Mean) {
    double data[] = {2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
    size_t n      = 8;

    // Mean = (2+4+4+4+5+5+7+9)/8 = 40/8 = 5.0
    EXPECT_DOUBLE_EQ(CBridge.math->mean(data, n), 5.0);
}

TEST(CBridgeMathStats, Variance) {
    double data[] = {2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
    size_t n      = 8;

    // Variance = ((2-5)^2 + 3*(4-5)^2 + 2*(5-5)^2 + (7-5)^2 + (9-5)^2) / 8
    //          = (9 + 3 + 0 + 4 + 16) / 8 = 32 / 8 = 4.0
    EXPECT_DOUBLE_EQ(CBridge.math->variance(data, n), 4.0);
}

TEST(CBridgeMathStats, StandardDeviation) {
    double data[] = {2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
    size_t n      = 8;

    // StdDev = sqrt(4.0) = 2.0
    EXPECT_DOUBLE_EQ(CBridge.math->standardDeviation(data, n), 2.0);
}

TEST(CBridgeMathStats, SigmaBounds) {
    double data[] = {10.0, 20.0, 30.0};
    size_t n      = 3;
    // Mean = 20.0
    // Variance = ((10-20)^2 + (20-20)^2 + (30-20)^2) / 3 = (100 + 0 + 100) / 3 = 66.66666666666667
    // StdDev = sqrt(200/3) = 8.16496580927726

    cbridge_sigma_bounds_t bounds = CBridge.math->calculateSigmaBounds(data, n, 1.0);
    double expected_std = sqrt(200.0 / 3.0);
    EXPECT_NEAR(bounds.lower_bound, 20.0 - expected_std, 1e-9);
    EXPECT_NEAR(bounds.upper_bound, 20.0 + expected_std, 1e-9);

    bounds = CBridge.math->calculateSigmaBounds(data, n, 2.0);
    EXPECT_NEAR(bounds.lower_bound, 20.0 - 2.0 * expected_std, 1e-9);
    EXPECT_NEAR(bounds.upper_bound, 20.0 + 2.0 * expected_std, 1e-9);
}

TEST(CBridgeMathStats, SigmaBoundsEdgeCases) {
    cbridge_sigma_bounds_t bounds = CBridge.math->calculateSigmaBounds(NULL, 0, 1.0);
    EXPECT_DOUBLE_EQ(bounds.lower_bound, 0.0);
    EXPECT_DOUBLE_EQ(bounds.upper_bound, 0.0);

    double single[] = {10.0};
    bounds = CBridge.math->calculateSigmaBounds(single, 1, 1.0);
    EXPECT_DOUBLE_EQ(bounds.lower_bound, 0.0);
    EXPECT_DOUBLE_EQ(bounds.upper_bound, 0.0);
}

TEST(CBridgeMathStats, SigmaBoundsAliases) {
    double data[] = {10.0, 20.0, 30.0};
    size_t n      = 3;
    
    // Testing the aliases
    cbridge_sigma_bounds_t lower = calculateSigmaLowerBound(data, n, 1.0);
    cbridge_sigma_bounds_t upper = calculateSigmaUpperBound(data, n, 1.0);
    
    EXPECT_DOUBLE_EQ(lower.lower_bound, upper.lower_bound);
    EXPECT_DOUBLE_EQ(lower.upper_bound, upper.upper_bound);
}

/**
 * @namespace CBridgeMathUtils
 * @brief Tests for utility functions (isNear, clamp).
 */
TEST(CBridgeMathUtils, IsNear) {
    EXPECT_TRUE(CBridge.math->isNear(1.0, 1.0000000001, 1e-9));
    EXPECT_FALSE(CBridge.math->isNear(1.0, 1.0000001, 1e-9));
}

TEST(CBridgeMathUtils, Clamp) {
    EXPECT_DOUBLE_EQ(CBridge.math->clamp(5.0, 0.0, 10.0), 5.0);
    EXPECT_DOUBLE_EQ(CBridge.math->clamp(-1.0, 0.0, 10.0), 0.0);
    EXPECT_DOUBLE_EQ(CBridge.math->clamp(11.0, 0.0, 10.0), 10.0);
}
