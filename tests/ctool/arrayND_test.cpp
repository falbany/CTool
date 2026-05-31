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

TEST(ArrayND_Construction, Default) {
    ctool::array::ArrayND<double> arr;
    EXPECT_TRUE(arr.empty());
    EXPECT_EQ(arr.rows(), 0);
    EXPECT_EQ(arr.cols(), 0);
    EXPECT_EQ(arr.size(), 0);
}

TEST(ArrayND_Construction, Sized) {
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

TEST(ArrayND_Construction, FromInitializerList) {
    ctool::array::ArrayND<double> arr = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
    EXPECT_EQ(arr.rows(), 2);
    EXPECT_EQ(arr.cols(), 3);
    EXPECT_EQ(arr.size(), 6);

    EXPECT_DOUBLE_EQ(arr(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(arr(1, 2), 6.0);
}

TEST(ArrayND_Construction, JaggedInitializerThrows) {
    auto jagged = []() { ctool::array::ArrayND<double> arr = {{1.0, 2.0}, {3.0, 4.0, 5.0}}; };
    EXPECT_THROW(jagged(), std::invalid_argument);
}

/* ============================================================================
 * Element Access
 * ============================================================================ */

TEST(ArrayND_Access, At) {
    ctool::array::ArrayND<int> arr = {{1, 2}, {3, 4}};
    EXPECT_EQ(arr.at(0, 1), 2);
    arr.at(1, 0) = 10;
    EXPECT_EQ(arr(1, 0), 10);

    EXPECT_THROW(arr.at(2, 0), std::out_of_range);
    EXPECT_THROW(arr.at(0, 2), std::out_of_range);
}

TEST(ArrayND_Access, DataPointer) {
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

TEST(ArrayND_Arithmetic, Addition) {
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

TEST(ArrayND_Arithmetic, Subtraction) {
    ctool::array::ArrayND<double> a = {{10, 20}, {30, 40}};
    ctool::array::ArrayND<double> b = {{1, 2}, {3, 4}};

    auto res = a - b;
    EXPECT_DOUBLE_EQ(res(0, 0), 9);
    EXPECT_DOUBLE_EQ(res(1, 1), 36);

    auto res_scalar = a - 5.0;
    EXPECT_DOUBLE_EQ(res_scalar(0, 0), 5);
}

TEST(ArrayND_Arithmetic, Multiplication) {
    ctool::array::ArrayND<double> a = {{1, 2}, {3, 4}};
    ctool::array::ArrayND<double> b = {{5, 6}, {7, 8}};

    auto res = a * b;
    EXPECT_DOUBLE_EQ(res(0, 0), 5);
    EXPECT_DOUBLE_EQ(res(1, 1), 32);

    auto res_scalar = a * 2.0;
    EXPECT_DOUBLE_EQ(res_scalar(0, 0), 2);
    EXPECT_DOUBLE_EQ(res_scalar(1, 1), 8);
}

TEST(ArrayND_Arithmetic, Division) {
    ctool::array::ArrayND<double> a   = {{10, 20}, {30, 40}};
    auto                          res = a / 2.0;
    EXPECT_DOUBLE_EQ(res(0, 0), 5);
    EXPECT_DOUBLE_EQ(res(1, 1), 20);
}

/* ============================================================================
 * Universal Functions (Ufuncs)
 * ============================================================================ */

TEST(ArrayND_Ufuncs, Sqrt) {
    ctool::array::ArrayND<double> a   = {{1, 4}, {9, 16}};
    auto                          res = ctool::array::sqrt(a);
    EXPECT_DOUBLE_EQ(res(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(res(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(res(1, 0), 3.0);
    EXPECT_DOUBLE_EQ(res(1, 1), 4.0);
}

TEST(ArrayND_Ufuncs, Abs) {
    ctool::array::ArrayND<int> a   = {{-1, 2}, {-3, -4}};
    auto                       res = ctool::array::abs(a);
    EXPECT_EQ(res(0, 0), 1);
    EXPECT_EQ(res(1, 0), 3);
    EXPECT_EQ(res(1, 1), 4);
}

/* ============================================================================
 * Reductions
 * ============================================================================ */

TEST(ArrayND_Reductions, Basic) {
    ctool::array::ArrayND<double> a = {{1, 2}, {3, 4}};
    EXPECT_DOUBLE_EQ(ctool::array::sum(a), 10.0);
    EXPECT_DOUBLE_EQ(ctool::array::mean(a), 2.5);
    EXPECT_DOUBLE_EQ(ctool::array::max(a), 4.0);
    EXPECT_DOUBLE_EQ(ctool::array::min(a), 1.0);
}

TEST(ArrayND_Reductions, Empty) {
    ctool::array::ArrayND<double> empty;
    EXPECT_DOUBLE_EQ(ctool::array::sum(empty), 0.0);
    EXPECT_DOUBLE_EQ(ctool::array::mean(empty), 0.0);
    EXPECT_THROW(ctool::array::max(empty), std::runtime_error);
    EXPECT_THROW(ctool::array::min(empty), std::runtime_error);
}

/* ============================================================================
 * Shape & Utility
 * ============================================================================ */

TEST(ArrayND_Utility, Shape) {
    ctool::array::ArrayND<int> arr(5, 10);
    auto [rows, cols] = arr.shape();
    EXPECT_EQ(rows, 5);
    EXPECT_EQ(cols, 10);
}

TEST(ArrayND_Utility, Clear) {
    ctool::array::ArrayND<int> arr = {{1, 2}, {3, 4}};
    EXPECT_FALSE(arr.empty());
    arr.clear();
    EXPECT_TRUE(arr.empty());
    EXPECT_EQ(arr.size(), 0);
}

TEST(ArrayND_Utility, Iterators) {
    ctool::array::ArrayND<int> arr = {{1, 2}, {3, 4}};
    int                       sum = 0;
    for (auto val : arr) {
        sum += val;
    }
    EXPECT_EQ(sum, 10);
}

/* ============================================================================
 * Additional Ufuncs
 * ============================================================================ */

TEST(ArrayND_Ufuncs, Sin) {
    ctool::array::ArrayND<double> a   = {{0.0, M_PI / 2.0}};
    auto                          res = ctool::array::sin(a);
    EXPECT_NEAR(res(0, 0), 0.0, 1e-9);
    EXPECT_NEAR(res(0, 1), 1.0, 1e-9);
}

/* ============================================================================
 * Edge Cases & Error Handling
 * ============================================================================ */

TEST(ArrayND_Arithmetic, MismatchedShapes) {
    ctool::array::ArrayND<double> a(2, 2);
    ctool::array::ArrayND<double> b(3, 3);
    EXPECT_THROW(a + b, std::invalid_argument);
    EXPECT_THROW(a - b, std::invalid_argument);
    EXPECT_THROW(a * b, std::invalid_argument);
}

