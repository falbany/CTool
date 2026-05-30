/**
 * @file array2D_test.cpp
 * @author Florent ALBANY - FAL
 * @brief Unit tests for Array2D class.
 * @version 1.0
 * @date 2026-05-30
 */

#include "ctool/array2D.hpp"
#include "gtest/gtest.h"

namespace {

    // ============================================================================
    // CONSTRUCTORS
    // ============================================================================

    TEST(Array2D_Construction, Default) {
        ctool::array::Array2D<int> arr;
        EXPECT_EQ(arr.rows(), 0);
        EXPECT_EQ(arr.cols(), 0);
        EXPECT_TRUE(arr.empty());
    }

    TEST(Array2D_Construction, SizedWithValue) {
        ctool::array::Array2D<double> arr(3, 4, 1.5);
        EXPECT_EQ(arr.rows(), 3);
        EXPECT_EQ(arr.cols(), 4);
        EXPECT_EQ(arr.size(), 12);
        EXPECT_FALSE(arr.empty());

        for (size_t r = 0; r < arr.rows(); ++r) {
            for (size_t c = 0; c < arr.cols(); ++c) {
                EXPECT_EQ(arr.at(r, c), 1.5);
            }
        }
    }

    TEST(Array2D_Construction, FromVector) {
        std::vector<std::vector<int>> data = {{1, 2}, {3, 4}, {5, 6}};
        ctool::array::Array2D<int>    arr(data);
        EXPECT_EQ(arr.rows(), 3);
        EXPECT_EQ(arr.cols(), 2);
        EXPECT_EQ(arr.at(1, 1), 4);

        std::vector<std::vector<int>> jagged = {{1, 2}, {3}};
        EXPECT_THROW(ctool::array::Array2D<int> arr2(jagged), std::invalid_argument);
    }

    TEST(Array2D_Access, IndividualCells) {
        ctool::array::Array2D<int> arr(3, 3, 10);
        arr.at(1, 1) = 99;
        EXPECT_EQ(arr.at(1, 1), 99);
        EXPECT_EQ(arr.at(0, 0), 10);    // Check isolation
        EXPECT_EQ(arr.at(2, 2), 10);
    }

    TEST(Array2D_Access, BoundsChecking) {
        ctool::array::Array2D<int> arr(2, 2, 0);
        arr.at(0, 1) = 42;
        EXPECT_EQ(arr.at(0, 1), 42);
        EXPECT_THROW(arr.at(2, 0), std::out_of_range);
        EXPECT_THROW(arr.at(0, 2), std::out_of_range);
    }

    TEST(Array2D_Access, OperatorParens) {
        ctool::array::Array2D<int> arr(2, 2, 0);
        arr(1, 1) = 7;
        EXPECT_EQ(arr(1, 1), 7);
    }

    // ============================================================================
    // OPERATIONS
    // ============================================================================

    TEST(Array2D_Operations, FillRow) {
        ctool::array::Array2D<int> arr(3, 3, 0);
        arr.fillRow(1, 5);
        EXPECT_EQ(arr.at(1, 0), 5);
        EXPECT_EQ(arr.at(1, 1), 5);
        EXPECT_EQ(arr.at(1, 2), 5);
        EXPECT_EQ(arr.at(0, 0), 0);
        EXPECT_EQ(arr.at(2, 0), 0);
    }

    TEST(Array2D_Operations, FillColumn) {
        ctool::array::Array2D<int> arr(3, 3, 0);
        arr.fillColumn(0, 9);
        EXPECT_EQ(arr.at(0, 0), 9);
        EXPECT_EQ(arr.at(1, 0), 9);
        EXPECT_EQ(arr.at(2, 0), 9);
        EXPECT_EQ(arr.at(0, 1), 0);
        EXPECT_EQ(arr.at(1, 1), 0);
    }

    TEST(Array2D_Operations, FillAndClear) {
        // Consolidated test name/structure if needed later
        ctool::array::Array2D<int> arr(2, 2, 1);
        arr.clear();
        EXPECT_TRUE(arr.empty());
        EXPECT_EQ(arr.rows(), 0);
        EXPECT_EQ(arr.cols(), 0);
    }

    TEST(Array2D_Operations, Resize) {
        ctool::array::Array2D<int> arr(2, 2, 1);
        arr.resize(3, 3, 5);
        EXPECT_EQ(arr.rows(), 3);
        EXPECT_EQ(arr.cols(), 3);
        EXPECT_EQ(arr.at(0, 0), 1);    // Preserved
        EXPECT_EQ(arr.at(2, 2), 5);    // New
    }

    TEST(Array2D_Operations, Math) {
        ctool::array::Array2D<double> arr(2, 2, 10.0);
        arr.scale(2.0).add(5.0);
        EXPECT_DOUBLE_EQ(arr.at(0, 0), 25.0);
    }

    TEST(Array2D_Operations, RowOps) {
        ctool::array::Array2D<int> arr(2, 2, 1);
        arr.pushRow(2, 8);
        EXPECT_EQ(arr.rows(), 3);
        EXPECT_EQ(arr.at(2, 0), 8);
        EXPECT_EQ(arr.at(2, 1), 8);
    }

}    // namespace
