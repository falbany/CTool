/**
 * @file num_test.cpp
 * @author Florent ALBANY - FAL
 * @brief Unit tests for ctool::array::ArrayND using Google Test.
 * @version 1.0
 * @date 2026-05-30
 */

#include <gtest/gtest.h>
#include "ctool/arrayND.hpp"

/* ============================================================================
 * Construction & Basic Properties
 * ============================================================================ */

TEST(CtArrayNDTest, DefaultConstructor) {
    ctool::array::ArrayND<double> arr;
    EXPECT_TRUE(arr.empty());
    EXPECT_EQ(arr.rows(), 0);
    EXPECT_EQ(arr.cols(), 0);
    EXPECT_EQ(arr.size(), 0);
}

TEST(CtArrayNDTest, SizeConstructor) {
    ctool::array::ArrayND<int> arr(3, 4);
    EXPECT_FALSE(arr.empty());
    EXPECT_EQ(arr.rows(), 3);
    EXPECT_EQ(arr.cols(), 4);
    EXPECT_EQ(arr.size(), 12);

    // Check initialization to zero
    for (size_t i = 0; i < arr.rows(); ++i) {
        for (size_t j = 0; j < arr.cols(); ++j) {
            EXPECT_EQ(arr(i, j), 0);
        }
    }
}

TEST(CtArrayNDTest, InitializerListConstructor) {
    ctool::array::ArrayND<double> arr = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
    EXPECT_EQ(arr.rows(), 2);
    EXPECT_EQ(arr.cols(), 3);
    EXPECT_EQ(arr.size(), 6);

    EXPECT_DOUBLE_EQ(arr(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(arr(1, 2), 6.0);
}

TEST(CtArrayNDTest, JaggedInitializerListThrows) {
    auto jagged = []() { ctool::array::ArrayND<double> arr = {{1.0, 2.0}, {3.0, 4.0, 5.0}}; };
    EXPECT_THROW(jagged(), std::invalid_argument);
}

/* ============================================================================
 * Element Access
 * ============================================================================ */

TEST(CtArrayNDTest, AtAccess) {
    ctool::array::ArrayND<int> arr = {{1, 2}, {3, 4}};
    EXPECT_EQ(arr.at(0, 1), 2);
    arr.at(1, 0) = 10;
    EXPECT_EQ(arr(1, 0), 10);

    EXPECT_THROW(arr.at(2, 0), std::out_of_range);
    EXPECT_THROW(arr.at(0, 2), std::out_of_range);
}

TEST(CtArrayNDTest, DataPointer) {
    ctool::array::ArrayND<int> arr = {{1, 2}, {3, 4}};
    int*                       ptr = arr.data();
    EXPECT_EQ(ptr[0], 1);
    EXPECT_EQ(ptr[1], 2);
    EXPECT_EQ(ptr[2], 3);
    EXPECT_EQ(ptr[3], 4);
}

/* ============================================================================
 * Arithmetic Operators
 * ============================================================================ */

TEST(CtArrayNDTest, Addition) {
    ctool::array::ArrayND<double> a = {{1, 2}, {3, 4}};
    ctool::array::ArrayND<double> b = {{5, 6}, {7, 8}};

    auto res = a + b;
    EXPECT_DOUBLE_EQ(res(0, 0), 6);
    EXPECT_DOUBLE_EQ(res(1, 1), 12);

    auto res_scalar = a + 10.0;
    EXPECT_DOUBLE_EQ(res_scalar(0, 0), 11);
    EXPECT_DOUBLE_EQ(res_scalar(1, 1), 14);

    auto res_scalar_rev = 10.0 + a;
    EXPECT_DOUBLE_EQ(res_scalar_rev(0, 0), 11);
}

TEST(CtArrayNDTest, Subtraction) {
    ctool::array::ArrayND<double> a = {{10, 20}, {30, 40}};
    ctool::array::ArrayND<double> b = {{1, 2}, {3, 4}};

    auto res = a - b;
    EXPECT_DOUBLE_EQ(res(0, 0), 9);
    EXPECT_DOUBLE_EQ(res(1, 1), 36);

    auto res_scalar = a - 5.0;
    EXPECT_DOUBLE_EQ(res_scalar(0, 0), 5);
}

TEST(CtArrayNDTest, Multiplication) {
    ctool::array::ArrayND<double> a = {{1, 2}, {3, 4}};
    ctool::array::ArrayND<double> b = {{5, 6}, {7, 8}};

    auto res = a * b;
    EXPECT_DOUBLE_EQ(res(0, 0), 5);
    EXPECT_DOUBLE_EQ(res(1, 1), 32);

    auto res_scalar = a * 2.0;
    EXPECT_DOUBLE_EQ(res_scalar(0, 0), 2);
    EXPECT_DOUBLE_EQ(res_scalar(1, 1), 8);
}

TEST(CtArrayNDTest, Division) {
    ctool::array::ArrayND<double> a   = {{10, 20}, {30, 40}};
    auto                          res = a / 2.0;
    EXPECT_DOUBLE_EQ(res(0, 0), 5);
    EXPECT_DOUBLE_EQ(res(1, 1), 20);
}

/* ============================================================================
 * Universal Functions (Ufuncs)
 * ============================================================================ */

TEST(CtArrayNDTest, UfuncSqrt) {
    ctool::array::ArrayND<double> a   = {{1, 4}, {9, 16}};
    auto                          res = ctool::array::sqrt(a);
    EXPECT_DOUBLE_EQ(res(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(res(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(res(1, 0), 3.0);
    EXPECT_DOUBLE_EQ(res(1, 1), 4.0);
}

TEST(CtArrayNDTest, UfuncAbs) {
    ctool::array::ArrayND<int> a   = {{-1, 2}, {-3, -4}};
    auto                       res = ctool::array::abs(a);
    EXPECT_EQ(res(0, 0), 1);
    EXPECT_EQ(res(1, 0), 3);
    EXPECT_EQ(res(1, 1), 4);
}

/* ============================================================================
 * Reductions
 * ============================================================================ */

TEST(CtArrayNDTest, Reductions) {
    ctool::array::ArrayND<double> a = {{1, 2}, {3, 4}};
    EXPECT_DOUBLE_EQ(ctool::array::sum(a), 10.0);
    EXPECT_DOUBLE_EQ(ctool::array::mean(a), 2.5);
    EXPECT_DOUBLE_EQ(ctool::array::max(a), 4.0);
    EXPECT_DOUBLE_EQ(ctool::array::min(a), 1.0);
}

TEST(CtArrayNDTest, EmptyReductions) {
    ctool::array::ArrayND<double> empty;
    EXPECT_DOUBLE_EQ(ctool::array::sum(empty), 0.0);
    EXPECT_DOUBLE_EQ(ctool::array::mean(empty), 0.0);
    EXPECT_THROW(ctool::array::max(empty), std::runtime_error);
    EXPECT_THROW(ctool::array::min(empty), std::runtime_error);
}
