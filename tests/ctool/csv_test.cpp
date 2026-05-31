/**
 * @file csv_test.cpp
 * @author GitHub Copilot
 * @brief Comprehensive test suite for CTool csv module (/csv) using Google Test.
 * @version 1.0
 * @date 2026-06-01
 */

#include "ctool/csv.hpp"
#include "ctool/file.hpp"
#include <gtest/gtest.h>

extern "C" {
// If testing CBridge components, include here
}

namespace {
    const std::string TEST_CSV_FILE = "test_data.csv";
}

TEST(CsvWriter, ReadWriteSimple) {
    {
        ctool::csv::Writer writer(TEST_CSV_FILE);
        writer.setHeader({"ID", "Name", "Score"});
        writer.writeRow(std::vector<std::string>{"1", "Alice", "95"});
        writer.writeRow(std::vector<int>{2, 42, 88});    // test templates
    }

    auto data = ctool::csv::read(TEST_CSV_FILE, ',', '"', false);
    ASSERT_EQ(data.size(), 3);
    EXPECT_EQ(data[0][0], "ID");
    EXPECT_EQ(data[0][1], "Name");
    EXPECT_EQ(data[0][2], "Score");

    EXPECT_EQ(data[1][0], "1");
    EXPECT_EQ(data[1][1], "Alice");
    EXPECT_EQ(data[1][2], "95");

    EXPECT_EQ(data[2][0], "2");
    EXPECT_EQ(data[2][1], "42");
    EXPECT_EQ(data[2][2], "88");

    ctool::file::remove(TEST_CSV_FILE);
}

TEST(CsvWriter, QuotesAndDelimiters) {
    {
        ctool::csv::Writer writer(TEST_CSV_FILE);
        writer.setHeader({"Item", "Description"});
        writer.writeRow(std::vector<std::string>{"Apple", "Red, juicy"});
        writer.writeRow(std::vector<std::string>{"Banana", "Has \"quotes\" inside"});
        writer.writeRow(std::vector<std::string>{"Cherry", "Line1\nLine2"});
    }

    auto data = ctool::csv::read(TEST_CSV_FILE, ',', '"', false);
    ASSERT_EQ(data.size(), 4);

    EXPECT_EQ(data[1][1], "Red, juicy");
    EXPECT_EQ(data[2][1], "Has \"quotes\" inside");
    EXPECT_EQ(data[3][1], "Line1\nLine2");

    ctool::file::remove(TEST_CSV_FILE);
}

TEST(CsvParser, ReadSkipHeader) {
    {
        ctool::csv::Writer writer(TEST_CSV_FILE);
        writer.setHeader({"A", "B", "C"});
        writer.writeRow(std::vector<int>{1, 2, 3});
    }

    auto data = ctool::csv::read(TEST_CSV_FILE, ',', '"', true);    // skipHeader = true
    ASSERT_EQ(data.size(), 1);
    EXPECT_EQ(data[0][0], "1");

    ctool::file::remove(TEST_CSV_FILE);
}

TEST(CsvWriter, AppendMode) {
    {
        ctool::csv::Writer writer(TEST_CSV_FILE, ',', '"', false);
        writer.setHeader({"H1"});
        writer.writeRow(std::vector<std::string>{"Row1"});
    }
    {
        ctool::csv::Writer writer(TEST_CSV_FILE, ',', '"', true);
        writer.writeRow(std::vector<std::string>{"Row2"});
    }

    auto data = ctool::csv::read(TEST_CSV_FILE, ',', '"', false);
    ASSERT_EQ(data.size(), 3);
    EXPECT_EQ(data[0][0], "H1");
    EXPECT_EQ(data[1][0], "Row1");
    EXPECT_EQ(data[2][0], "Row2");

    ctool::file::remove(TEST_CSV_FILE);
}

TEST(CsvBulk, WriteAll) {
    std::vector<std::vector<std::string>> grid = {{"A", "B", "C"}, {"1", "2", "3"}, {"4", "5", "6"}};

    bool result = ctool::csv::writeAll(TEST_CSV_FILE, grid);
    EXPECT_TRUE(result);

    auto data = ctool::csv::read(TEST_CSV_FILE, ',', '"', false);
    ASSERT_EQ(data.size(), 3);
    EXPECT_EQ(data[1][1], "2");
    EXPECT_EQ(data[2][2], "6");

    ctool::file::remove(TEST_CSV_FILE);
}

TEST(CsvQueries, GetColumn) {
    std::vector<std::vector<std::string>> grid = {{"A", "B", "C"}, {"1", "2", "3"}, {"4", "5", "6"}};

    auto col1 = ctool::csv::getColumn(grid, 1);
    ASSERT_EQ(col1.size(), 3);
    EXPECT_EQ(col1[0], "B");
    EXPECT_EQ(col1[1], "2");
    EXPECT_EQ(col1[2], "5");
}

TEST(CsvQueries, GetColumnByName) {
    std::vector<std::vector<std::string>> grid = {{"ID", "Name", "Age"}, {"1", "Alice", "25"}, {"2", "Bob", "30"}};

    ctool::csv::writeAll(TEST_CSV_FILE, grid);

    auto names = ctool::csv::getColumnByName(TEST_CSV_FILE, "Name");
    ASSERT_EQ(names.size(), 2);
    EXPECT_EQ(names[0], "Alice");
    EXPECT_EQ(names[1], "Bob");

    auto absent = ctool::csv::getColumnByName(TEST_CSV_FILE, "Salary");
    EXPECT_TRUE(absent.empty());

    ctool::file::remove(TEST_CSV_FILE);
}

TEST(CsvQueries, GetRow) {
    std::vector<std::vector<std::string>> grid = {{"ID", "Name", "Age"}, {"1", "Alice", "25"}, {"2", "Bob", "30"}};

    auto row1 = ctool::csv::getRow(grid, 1);
    ASSERT_EQ(row1.size(), 3);
    EXPECT_EQ(row1[0], "1");
    EXPECT_EQ(row1[1], "Alice");
    EXPECT_EQ(row1[2], "25");

    auto outOfBounds = ctool::csv::getRow(grid, 5);
    EXPECT_TRUE(outOfBounds.empty());
}

TEST(CsvQueries, GetRowByName) {
    std::vector<std::vector<std::string>> grid = {{"ID", "Name", "Age"}, {"1", "Alice", "25"}, {"2", "Bob", "30"}};

    ctool::csv::writeAll(TEST_CSV_FILE, grid);

    // Search in column 1 (Name)
    auto aliceRow = ctool::csv::getRowByName(TEST_CSV_FILE, "Alice", 1);
    ASSERT_EQ(aliceRow.size(), 3);
    EXPECT_EQ(aliceRow[0], "1");
    EXPECT_EQ(aliceRow[2], "25");

    // Search in column 0 (ID) - default
    auto bobRow = ctool::csv::getRowByName(TEST_CSV_FILE, "2");
    ASSERT_EQ(bobRow.size(), 3);
    EXPECT_EQ(bobRow[1], "Bob");

    auto absent = ctool::csv::getRowByName(TEST_CSV_FILE, "Charlie", 1);
    EXPECT_TRUE(absent.empty());

    ctool::file::remove(TEST_CSV_FILE);
}
