/**
 * @file dataframe_test.cpp
 * @author Florent ALBANY - FAL
 * @brief Comprehensive unit tests for ctool::data::DataFrame using Google Test.
 * @version 1.1
 * @date 2026-05-31
 */

#include <gtest/gtest.h>
#include "ctool/dataframe.hpp"
#include <vector>
#include <string>
#include <stdexcept>

using namespace ctool::data;

/**
 * @brief Helper to create a standard DataFrame for testing.
 */
static DataFrame createTestDF() {
    DataFrame df;
    df.addColumn("ID");
    df.addColumn("Name");
    df.addColumn("Score");
    df.pushRow({Cell(101), Cell("Alice"), Cell(95.5)});
    df.pushRow({Cell(102), Cell("Bob"), Cell(88)});
    df.pushRow({Cell(103), Cell("Charlie"), Cell(92.3)});
    return df;
}

/* ============================================================================
 * DataFrame Core Tests
 * ============================================================================ */

TEST(DataFrameCore, Constructor) {
    DataFrame df;
    EXPECT_EQ(df.rows(), 0) << "New DataFrame should have 0 rows";
    EXPECT_EQ(df.cols(), 0) << "New DataFrame should have 0 columns";
}

TEST(DataFrameCore, AddColumn) {
    DataFrame df;
    df.addColumn("ColA");
    df.addColumn("ColB");
    EXPECT_EQ(df.cols(), 2) << "Column count should be 2 after additions";
    ASSERT_GE(df.columnNames().size(), 2);
    EXPECT_EQ(df.columnNames().at(0), "ColA");
    EXPECT_EQ(df.columnNames().at(1), "ColB");
}

TEST(DataFrameCore, DuplicateColumnNames) {
    DataFrame df;
    df.addColumn("ColX");
    df.addColumn("ColX");
    EXPECT_EQ(df.cols(), 2) << "Implementation appends columns even if names duplicate";
}

TEST(DataFrameCore, PushRow) {
    auto df = createTestDF();
    EXPECT_EQ(df.rows(), 3) << "Initial rows should be 3";

    df.pushRow({Cell(104), Cell("David"), Cell(77.0)});
    EXPECT_EQ(df.rows(), 4) << "Row count should increase to 4";
}

TEST(DataFrameCore, MismatchedRowSize) {
    auto df = createTestDF();
    // Too few
    EXPECT_THROW(df.pushRow({Cell(105), Cell("Eve")}), std::invalid_argument) << "Should throw invalid_argument on missing columns";
    // Too many
    EXPECT_THROW(df.pushRow({Cell(105), Cell("Eve"), Cell(1.0), Cell("Extra")}), std::invalid_argument) << "Should throw invalid_argument on extra columns";
}

TEST(DataFrameCore, Clear) {
    auto df = createTestDF();
    df.clear();
    EXPECT_EQ(df.rows(), 0);
    EXPECT_EQ(df.cols(), 0);
    EXPECT_TRUE(df.columnNames().empty());
}

/* ============================================================================
 * DataFrame Access Tests
 * ============================================================================ */

TEST(DataFrameAccess, At) {
    auto df = createTestDF();
    EXPECT_DOUBLE_EQ(df.at(0, "Score").asDouble(), 95.5) << "Correct score for Alice";
    EXPECT_EQ(df.at(1, "Name").asString(), "Bob") << "Correct name for row 1";
    EXPECT_EQ(df.at(2, "ID").asInt(), 103) << "Correct ID for row 2";
}

TEST(DataFrameAccess, AtBoundsChecking) {
    auto df = createTestDF();
    EXPECT_THROW(df.at(3, "ID"), std::out_of_range) << "Row index 3 is out of bounds";
    EXPECT_THROW(df.at(0, "Missing"), std::out_of_range) << "Column 'Missing' does not exist";
}

TEST(DataFrameAccess, GetRow) {
    auto              df  = createTestDF();
    std::vector<Cell> row = df.getRow(0);
    ASSERT_EQ(row.size(), 3);
    EXPECT_EQ(row[0].asInt(), 101);
    EXPECT_EQ(row[1].asString(), "Alice");
}

TEST(DataFrameAccess, GetColumn) {
    auto              df  = createTestDF();
    std::vector<Cell> col = df.getColumn("Name");
    ASSERT_EQ(col.size(), 3);
    EXPECT_EQ(col[0].asString(), "Alice");
    EXPECT_EQ(col[1].asString(), "Bob");
    EXPECT_EQ(col[2].asString(), "Charlie");
}

/* ============================================================================
 * DataFrame Slicing Tests
 * ============================================================================ */

TEST(DataFrameSlicing, Head) {
    auto      df   = createTestDF();
    DataFrame top2 = df.head(2);
    EXPECT_EQ(top2.rows(), 2);
    EXPECT_EQ(top2.at(1, "Name").asString(), "Bob");

    DataFrame all = df.head(10);
    EXPECT_EQ(all.rows(), 3);
}

/* ============================================================================
 * Cell Type & Conversion Tests
 * ============================================================================ */

TEST(CellTypes, Conversions) {
    Cell c_int(42);
    EXPECT_EQ(c_int.type(), Cell::Type::INT);
    EXPECT_EQ(c_int.asInt(), 42);
    EXPECT_DOUBLE_EQ(c_int.asDouble(), 42.0);

    Cell c_dbl(3.14);
    EXPECT_EQ(c_dbl.type(), Cell::Type::DOUBLE);
    EXPECT_NEAR(c_dbl.asDouble(), 3.14, 1e-9);
    EXPECT_EQ(c_dbl.asInt(), 3) << "Double to Int should truncate";

    Cell c_str("CTool");
    EXPECT_EQ(c_str.type(), Cell::Type::STRING);
    EXPECT_EQ(c_str.asString(), "CTool");
}

TEST(CellTypes, EmptyCell) {
    Cell empty;
    EXPECT_EQ(empty.type(), Cell::Type::EMPTY);
    EXPECT_TRUE(isNull(empty));
    EXPECT_THROW(empty.asInt(), std::runtime_error);
    EXPECT_THROW(empty.asString(), std::runtime_error);
}
