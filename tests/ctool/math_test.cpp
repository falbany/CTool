/**
 * @file math_test.cpp
 * @author Florent ALBANY - FAL
 * @brief Unit tests for ctool::math utilities.
 * @version 1.0
 * @date 2026-05-31
 */

#include <gtest/gtest.h>
#include "ctool/math.hpp"
#include <vector>
#include <cmath>

using namespace ctool::math;

/* ============================================================================
 * Basic Statistics Tests
 * ============================================================================ */

TEST(CtMathBasicStatsTest, MinMaxMean) {
    std::vector<double> data = {10.0, 2.0, 5.0, 8.0, 5.0};

    EXPECT_DOUBLE_EQ(min(data), 2.0);
    EXPECT_DOUBLE_EQ(max(data), 10.0);
    EXPECT_DOUBLE_EQ(mean(data), 6.0);

    // Empty vector
    std::vector<double> empty;
    EXPECT_DOUBLE_EQ(min(empty), 0.0);
    EXPECT_DOUBLE_EQ(max(empty), 0.0);
    EXPECT_DOUBLE_EQ(mean(empty), 0.0);
}

TEST(CtMathBasicStatsTest, VarianceAndStdDev) {
    std::vector<double> data = {2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
    // Mean = 5.0
    // Squared differences: (2-5)^2=9, (4-5)^2=1, (4-5)^2=1, (4-5)^2=1, (5-5)^2=0, (5-5)^2=0, (7-5)^2=4, (9-5)^2=16
    // Sum of squares = 9+1+1+1+0+0+4+16 = 32
    // Population Variance = 32 / 8 = 4.0
    // StdDev = sqrt(4.0) = 2.0

    EXPECT_NEAR(variance(data), 4.0, 1e-9);
    EXPECT_NEAR(standardDeviation(data), 2.0, 1e-9);

    // Less than 2 elements
    std::vector<double> single = {10.0};
    EXPECT_DOUBLE_EQ(variance(single), 0.0);
    EXPECT_DOUBLE_EQ(standardDeviation(single), 0.0);

    // Empty vector for variance/stdDev
    std::vector<double> empty_vals;
    EXPECT_DOUBLE_EQ(variance(empty_vals), 0.0);
    EXPECT_DOUBLE_EQ(standardDeviation(empty_vals), 0.0);
}

TEST(CtMathBasicStatsTest, Median) {
    // Odd size
    EXPECT_DOUBLE_EQ(median(std::vector<double>{3.0, 1.0, 2.0}), 2.0);
    // Even size
    EXPECT_DOUBLE_EQ(median(std::vector<double>{4.0, 1.0, 2.0, 3.0}), 2.5);
    // Empty
    EXPECT_DOUBLE_EQ(median(std::vector<double>{}), 0.0);
    // Integers - checking if it correctly handles (2+3)/2 = 2.5 cast back to int or correctly computed
    EXPECT_EQ(median(std::vector<int>{1, 2, 3, 4}), 2);    // 2.5 cast to int
}

TEST(CtMathBasicStatsTest, Sum) {
    EXPECT_DOUBLE_EQ(sum(std::vector<double>{1.1, 2.2, 3.3}), 6.6);
    EXPECT_EQ(sum(std::vector<int>{1, 2, 3}), 6);
    EXPECT_EQ(sum(std::vector<int>{}), 0);
}

TEST(CtMathBasicStatsTest, Percentile) {
    std::vector<double> data = {15, 20, 35, 40, 50};

    EXPECT_DOUBLE_EQ(percentile(data, 0), 15.0);
    EXPECT_DOUBLE_EQ(percentile(data, 100), 50.0);
    EXPECT_DOUBLE_EQ(percentile(data, 50), 35.0);
    EXPECT_DOUBLE_EQ(percentile(data, 40), 29.0);    // 15 + (35-15)*0.4? No, interpolation between 20 and 35.
    // rank = (40/100) * (5-1) = 0.4 * 4 = 1.6
    // lowerIdx = 1 (20), upperIdx = 2 (35), fraction = 0.6
    // 20 * 0.4 + 35 * 0.6 = 8 + 21 = 29.0

    EXPECT_THROW(percentile(data, -1), std::invalid_argument);
    EXPECT_THROW(percentile(data, 101), std::invalid_argument);
    EXPECT_DOUBLE_EQ(percentile(std::vector<double>{}, 50), 0.0);

    // Dataset with exactly 1 element
    std::vector<double> single = {42.0};
    EXPECT_DOUBLE_EQ(percentile(single, 50), 42.0);

    // Interpolation where percentile corresponds precisely to an index (lowerIdx == upperIdx)
    // using k=25% on 5 elements -> rank = 0.25 * 4 = 1.0 (exact index 1)
    EXPECT_DOUBLE_EQ(percentile(data, 25), 20.0);
}

/* ============================================================================
 * Scaling And Utility Tests
 * ============================================================================ */

TEST(CtMathUtilityTest, Clamp) {
    EXPECT_EQ(clamp(5, 1, 10), 5);
    EXPECT_EQ(clamp(0, 1, 10), 1);
    EXPECT_EQ(clamp(11, 1, 10), 10);
}

TEST(CtMathUtilityTest, IsNear) {
    EXPECT_TRUE(isNear(1.0, 1.0000000001));
    EXPECT_FALSE(isNear(1.0, 1.01));
    EXPECT_TRUE(isNear(1.0, 1.1, 0.2));
}

TEST(CtMathUtilityTest, MinMaxScale) {
    std::vector<double> data   = {10.0, 20.0, 30.0};
    auto                scaled = minMaxScale(data);
    ASSERT_EQ(scaled.size(), 3);
    EXPECT_DOUBLE_EQ(scaled[0], 0.0);
    EXPECT_DOUBLE_EQ(scaled[1], 0.5);
    EXPECT_DOUBLE_EQ(scaled[2], 1.0);

    // Constant data
    std::vector<double> constant = {5.0, 5.0, 5.0};
    auto                scaled_c = minMaxScale(constant);
    for (auto v : scaled_c) EXPECT_DOUBLE_EQ(v, 0.0);

    // Empty dataset
    std::vector<double> empty_scale;
    auto                scaled_e = minMaxScale(empty_scale);
    EXPECT_TRUE(scaled_e.empty());
}

/* ============================================================================
 * Regression Tests
 * ============================================================================ */

TEST(CtMathRegressionTest, LinearFit) {
    std::vector<double> x = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<double> y = {2.0, 4.0, 6.0, 8.0, 10.0};

    auto res = linearFit(x, y);
    EXPECT_NEAR(res.slope, 2.0, 1e-9);
    EXPECT_NEAR(res.intercept, 0.0, 1e-9);
    EXPECT_NEAR(res.rSquared, 1.0, 1e-9);

    // Noisy data
    std::vector<double> yn    = {2.1, 3.9, 6.2, 8.1, 9.8};
    auto                res_n = linearFit(x, yn);
    EXPECT_NEAR(res_n.slope, 1.95, 0.1);
    EXPECT_GT(res_n.rSquared, 0.9);

    // Mismatched sizes or too few points
    EXPECT_DOUBLE_EQ(linearFit({1.0}, {2.0}).slope, 0.0);
    EXPECT_DOUBLE_EQ(linearFit({1.0, 2.0}, {2.0}).slope, 0.0);

    // Singular matrix (zero variance in X)
    EXPECT_DOUBLE_EQ(linearFit({1.0, 1.0}, {2.0, 3.0}).slope, 0.0);
}

TEST(CtMathRegressionTest, LogLinearFit) {
    // y = 2 * exp(0.5 * x)
    // ln(y) = 0.5 * x + ln(2)
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {2.0, 2.0 * std::exp(0.5), 2.0 * std::exp(1.0)};

    auto res = logLinearFit(x, y);
    EXPECT_NEAR(res.slope, 0.5, 1e-9);
    EXPECT_NEAR(res.intercept, std::log(2.0), 1e-9);
    EXPECT_NEAR(res.rSquared, 1.0, 1e-9);

    // Negative values
    EXPECT_THROW(logLinearFit(x, std::vector<double>{1.0, -1.0, 1.0}), std::invalid_argument);

    // Mismatched sizes or too few points
    EXPECT_DOUBLE_EQ(logLinearFit({1.0}, {2.0}).slope, 0.0);
    EXPECT_DOUBLE_EQ(logLinearFit({1.0, 2.0}, {2.0}).slope, 0.0);

    // Singular matrix (zero variance in X)
    EXPECT_DOUBLE_EQ(logLinearFit({1.0, 1.0}, {2.0, 3.0}).slope, 0.0);
}

TEST(CtMathRegressionTest, PolynomialFit) {
    // y = 1 + 2x + 3x^2
    std::vector<double> x = {-1.0, 0.0, 1.0, 2.0};
    std::vector<double> y = {2.0, 1.0, 6.0, 17.0};

    auto coeffs = polynomialFit(x, y, 2);
    ASSERT_EQ(coeffs.size(), 3);
    EXPECT_NEAR(coeffs[0], 1.0, 1e-9);
    EXPECT_NEAR(coeffs[1], 2.0, 1e-9);
    EXPECT_NEAR(coeffs[2], 3.0, 1e-9);

    // Degree mismatch/limit
    EXPECT_THROW(polynomialFit(x, y, 5), std::invalid_argument);
    EXPECT_THROW(polynomialFit(x, y, 0), std::invalid_argument);

    // Not enough data points
    EXPECT_TRUE(polynomialFit({1.0}, {1.0}, 2).empty());

    // Singular system (e.g. all points same)
    // Degrees should be fine but solveLinearSystem might hit singular path
    auto coeffs_sing = polynomialFit({1.0, 1.0, 1.0}, {2.0, 2.0, 2.0}, 2);
    // Even if singular, solveLinearSystem skips diagonal updates for singular cols
    // but the result might be zeros or partials. We just ensure it doesn't crash.
}

TEST(CtMathRegressionTest, Residuals) {
    std::vector<double> x = {1, 2, 3};
    std::vector<double> y = {2, 4, 7};    // Predicted y for slope=2, intercept=0 would be 2, 4, 6

    auto res = residuals(x, y, 2.0, 0.0);
    ASSERT_EQ(res.size(), 3);
    EXPECT_DOUBLE_EQ(res[0], 0.0);
    EXPECT_DOUBLE_EQ(res[1], 0.0);
    EXPECT_DOUBLE_EQ(res[2], 1.0);

    // Mismatched sizes
    EXPECT_THROW(residuals({1.0}, {1.0, 2.0}, 1.0, 0.0), std::invalid_argument);
}
