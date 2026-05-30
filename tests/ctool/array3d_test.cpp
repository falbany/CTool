/**
 * @file array3d_test.cpp
 * @author Florent ALBANY - FAL
 * @brief Unit tests for Array3D class.
 * @version 1.0
 * @date 2026-05-30
 */

#include "ctool/array3d.hpp"
#include "gtest/gtest.h"

namespace {

    // ============================================================================
    // CONSTRUCTORS
    // ============================================================================

    TEST(Array3D_Construction, Default) {
        ctool::array::Array3D<int> arr;
        EXPECT_EQ(arr.depth(), 0);
        EXPECT_EQ(arr.rows(), 0);
        EXPECT_EQ(arr.cols(), 0);
        EXPECT_TRUE(arr.empty());
    }

    TEST(Array3D_Construction, SizedWithValue) {
        ctool::array::Array3D<double> arr(2, 3, 4, 2.5);
        EXPECT_EQ(arr.depth(), 2);
        EXPECT_EQ(arr.rows(), 3);
        EXPECT_EQ(arr.cols(), 4);
        EXPECT_EQ(arr.size(), 24);
        EXPECT_FALSE(arr.empty());

        for (size_t d = 0; d < arr.depth(); ++d) {
            for (size_t r = 0; r < arr.rows(); ++r) {
                for (size_t c = 0; c < arr.cols(); ++c) {
                    EXPECT_EQ(arr.at(d, r, c), 2.5);
                }
            }
        }
    }

    TEST(Array3D_Construction, FromVector) {
        std::vector<std::vector<std::vector<int>>> data = {{{1, 2}}, {{3, 4}}};
        ctool::array::Array3D<int>                 arr(data);
        EXPECT_EQ(arr.depth(), 2);
        EXPECT_EQ(arr.rows(), 1);
        EXPECT_EQ(arr.cols(), 2);
        EXPECT_EQ(arr.at(1, 0, 1), 4);
    }

    TEST(Array3D_Access, IndividualCells) {
        ctool::array::Array3D<int> arr(2, 2, 2, 10);
        arr.at(1, 0, 1) = 99;
        EXPECT_EQ(arr.at(1, 0, 1), 99);
        EXPECT_EQ(arr.at(0, 0, 0), 10);    // Check isolation
        EXPECT_EQ(arr.at(1, 1, 1), 10);
    }

    TEST(Array3D_Access, BoundsChecking) {
        ctool::array::Array3D<int> arr(1, 2, 2, 0);
        arr.at(0, 0, 1) = 42;
        EXPECT_EQ(arr.at(0, 0, 1), 42);
        EXPECT_THROW(arr.at(1, 0, 0), std::out_of_range);
        EXPECT_THROW(arr.at(0, 2, 0), std::out_of_range);
        EXPECT_THROW(arr.at(0, 0, 2), std::out_of_range);
    }

    TEST(Array3D_Access, OperatorParens) {
        ctool::array::Array3D<int> arr(1, 1, 1, 0);
        arr(0, 0, 0) = 7;
        EXPECT_EQ(arr(0, 0, 0), 7);
    }

    // ============================================================================
    // OPERATIONS
    // ============================================================================

    TEST(Array3D_Operations, FillDepth) {
        ctool::array::Array3D<int> arr(2, 2, 2, 0);
        arr.fillDepth(1, 5);
        EXPECT_EQ(arr.at(1, 0, 0), 5);
        EXPECT_EQ(arr.at(1, 1, 1), 5);
        EXPECT_EQ(arr.at(0, 0, 0), 0);
        EXPECT_EQ(arr.at(0, 1, 1), 0);
    }

    TEST(Array3D_Operations, FillRow) {
        ctool::array::Array3D<int> arr(2, 2, 2, 0);
        arr.fillRow(0, 8);
        EXPECT_EQ(arr.at(0, 0, 0), 8);
        EXPECT_EQ(arr.at(0, 0, 1), 8);
        EXPECT_EQ(arr.at(1, 0, 0), 8);
        EXPECT_EQ(arr.at(1, 0, 1), 8);
        EXPECT_EQ(arr.at(0, 1, 0), 0);
        EXPECT_EQ(arr.at(1, 1, 0), 0);
    }

    TEST(Array3D_Operations, FillColumn) {
        ctool::array::Array3D<int> arr(2, 2, 2, 0);
        arr.fillColumn(1, 9);
        EXPECT_EQ(arr.at(0, 0, 1), 9);
        EXPECT_EQ(arr.at(0, 1, 1), 9);
        EXPECT_EQ(arr.at(1, 0, 1), 9);
        EXPECT_EQ(arr.at(1, 1, 1), 9);
        EXPECT_EQ(arr.at(0, 0, 0), 0);
        EXPECT_EQ(arr.at(1, 1, 0), 0);
    }

    TEST(Array3D_Operations, FillAndClear) {
        ctool::array::Array3D<int> arr(2, 2, 2, 1);
        arr.clear();
        EXPECT_TRUE(arr.empty());
        EXPECT_EQ(arr.depth(), 0);
        EXPECT_EQ(arr.rows(), 0);
        EXPECT_EQ(arr.cols(), 0);
    }

    TEST(Array3D_Operations, Resize) {
        ctool::array::Array3D<int> arr(1, 1, 1, 1);
        arr.resize(2, 2, 2, 5);
        EXPECT_EQ(arr.depth(), 2);
        EXPECT_EQ(arr.at(0, 0, 0), 1);    // Preserved
        EXPECT_EQ(arr.at(1, 1, 1), 5);    // New
    }

    TEST(Array3D_Operations, Math) {
        ctool::array::Array3D<double> arr(1, 2, 2, 10.0);
        arr.scale(2.0).add(5.0);
        EXPECT_DOUBLE_EQ(arr.at(0, 0, 0), 25.0);
    }

    TEST(Array3D_Operations, MinMax) {
        ctool::array::Array3D<int> arr(1, 2, 2, 10);
        arr.at(0, 0, 0) = 1;
        arr.at(0, 1, 1) = 20;
        EXPECT_EQ(arr.min(), 1);
        EXPECT_EQ(arr.max(), 20);
    }

}    // namespace
