/**
 * @file file_test.cpp
 * @author Florent ALBANY - FAL
 * @brief Comprehensive test suite for CTool file system utilities (ctool::file) using Google Test.
 * @version 1.0
 * @date 2026-05-30
 */

#include <gtest/gtest.h>
#include "ctool/file.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>    // for std::find

/* ============================================================================
 * Helper to write a temporary file
 * ============================================================================ */
static void writeTempFile(const std::string& filename, const std::string& content) {
    std::ofstream out(filename, std::ios::binary);
    out << content;
}

/* ============================================================================
 * exists / remove Tests
 * ============================================================================ */

TEST(CtFileExistsRemove, ExistsAndRemoveFiles) {
    std::string testFile = "test_exists_remove.txt";

    // Ensure it doesn't exist
    ctool::file::remove(testFile);
    EXPECT_FALSE(ctool::file::exists(testFile));

    // Create file
    writeTempFile(testFile, "Hello, World!");
    EXPECT_TRUE(ctool::file::exists(testFile));

    // Remove file
    EXPECT_TRUE(ctool::file::remove(testFile));
    EXPECT_FALSE(ctool::file::exists(testFile));
}

/* ============================================================================
 * getSize Tests
 * ============================================================================ */

TEST(CtFileSize, GetFileSize) {
    std::string testFile = "test_size.txt";
    ctool::file::remove(testFile);

    // Non-existent file should return -1
    EXPECT_EQ(ctool::file::getSize(testFile), -1);

    // Write some content
    std::string content = "1234567890";    // 10 bytes
    writeTempFile(testFile, content);
    EXPECT_EQ(ctool::file::getSize(testFile), 10);

    // Cleanup
    ctool::file::remove(testFile);
}

/* ============================================================================
 * readAll / writeAll Tests
 * ============================================================================ */

TEST(CtFileReadWriteAll, ReadAndWriteAll) {
    std::string testFile = "test_read_write.txt";
    ctool::file::remove(testFile);

    std::string content = "Line 1\r\nLine 2\nLine 3\r";
    EXPECT_TRUE(ctool::file::writeAll(testFile, content));

    // readAll normalizes \r to run with findAndReplace
    std::string readContent = ctool::file::readAll(testFile);
    EXPECT_EQ(readContent, "Line 1\nLine 2\nLine 3");

    // Non-existent file read
    EXPECT_EQ(ctool::file::readAll("non_existent_file_xyz.txt"), "");

    // Cleanup
    ctool::file::remove(testFile);
}

TEST(CtFileReadWriteAll, WriteFailed) {
    // Attempting to write to a path that is a directory (should fail on most systems)
    EXPECT_FALSE(ctool::file::writeAll(".", "content"));
}

/* ============================================================================
 * readLines Tests
 * ============================================================================ */

TEST(CtFileReadLines, ReadLines) {
    std::string testFile = "test_readlines.txt";
    ctool::file::remove(testFile);

    std::string content = "Line 1\nLine 2\nLine 3\n";
    writeTempFile(testFile, content);

    std::vector<std::string> lines = ctool::file::readLines(testFile);
    ASSERT_EQ(lines.size(), 3);
    EXPECT_EQ(lines[0], "Line 1");
    EXPECT_EQ(lines[1], "Line 2");
    EXPECT_EQ(lines[2], "Line 3");

    // Clear and check empty or non-existent
    std::vector<std::string> emptyLines = ctool::file::readLines("non_existent_file_xyz.txt");
    EXPECT_TRUE(emptyLines.empty());

    // Cleanup
    ctool::file::remove(testFile);
}

TEST(CtFileReadLines, ReadLinesNoNewlineAtEnd) {
    std::string testFile = "test_readlines_nonl.txt";
    ctool::file::remove(testFile);

    writeTempFile(testFile, "Line 1\nLast Line");
    std::vector<std::string> lines = ctool::file::readLines(testFile);
    ASSERT_EQ(lines.size(), 2);
    EXPECT_EQ(lines[0], "Line 1");
    EXPECT_EQ(lines[1], "Last Line");

    ctool::file::remove(testFile);
}

/* ============================================================================
 * appendLine Tests
 * ============================================================================ */

TEST(CtFileAppendLine, AppendLine) {
    std::string testFile = "test_append.txt";
    ctool::file::remove(testFile);

    EXPECT_TRUE(ctool::file::appendLine(testFile, "First line"));
    EXPECT_TRUE(ctool::file::appendLine(testFile, "Second line"));

    std::vector<std::string> lines = ctool::file::readLines(testFile);
    ASSERT_EQ(lines.size(), 2);
    EXPECT_EQ(lines[0], "First line");
    EXPECT_EQ(lines[1], "Second line");

    // Cleanup
    ctool::file::remove(testFile);
}

TEST(CtFileAppendLine, AppendFailed) {
    // Path that cannot be created/opened as a file
    EXPECT_FALSE(ctool::file::appendLine("", "content"));
}

/* ============================================================================
 * getParameter Tests
 * ============================================================================ */

TEST(CtFileGetParameter, GetParameterParsing) {
    std::string testFile = "test_param.conf";
    ctool::file::remove(testFile);

    std::string configContent =
        "# This is a comment\n"
        "key1 = value1\n"
        "  key2  :  value2  \n"    // with colons and space
        "// Another comment\n"
        "key3   =   value3 # inline comment\n"
        "key4; value4\n"    // separator is ;
        "ignored_key_without_separator\n";

    writeTempFile(testFile, configContent);

    // Test with default separator "="
    EXPECT_EQ(ctool::file::getParameter(testFile, "key1"), "value1");
    EXPECT_EQ(ctool::file::getParameter(testFile, "key3"), "value3");

    // Test with separator ":"
    EXPECT_EQ(ctool::file::getParameter(testFile, "key2", ":"), "value2");

    // Test with non-existent key
    EXPECT_EQ(ctool::file::getParameter(testFile, "non_existent"), "");

    // Test with non-existent file
    EXPECT_EQ(ctool::file::getParameter("non_existent_file.conf", "key1"), "");

    // Cleanup
    ctool::file::remove(testFile);
}

/* ============================================================================
 * getWorkingDirectory Tests
 * ============================================================================ */

TEST(CtFileGetWorkingDirectory, GetWD) {
    std::string wd = ctool::file::getWorkingDirectory();
    EXPECT_FALSE(wd.empty());
}

/* ============================================================================
 * getExtension Tests
 * ============================================================================ */

TEST(CtFileGetExtension, GetExtensionOfPath) {
    EXPECT_EQ(ctool::file::getExtension("archive.tar.gz"), ".gz");
    EXPECT_EQ(ctool::file::getExtension("document.pdf"), ".pdf");
    EXPECT_EQ(ctool::file::getExtension("no_extension"), "");
    EXPECT_EQ(ctool::file::getExtension("path/to/file.cpp"), ".cpp");
    EXPECT_EQ(ctool::file::getExtension(".hidden_file"), ".hidden_file");
}

/* ============================================================================
 * getFiles Tests
 * ============================================================================ */

TEST(CtFileGetFiles, ListDirectoryFiles) {
    std::string testDirectory = ".";

    // Create some temp files in current directory to guarantee list results
    std::string f1 = "tmp_abc_123.test";
    std::string f2 = "tmp_abc_456.test";
    std::string f3 = "tmp_xyz_123.test";
    std::string f4 = "tmp_abc_123.other";

    writeTempFile(f1, "tmp");
    writeTempFile(f2, "tmp");
    writeTempFile(f3, "tmp");
    writeTempFile(f4, "tmp");

    // Match prefix only
    std::vector<std::string> prefixMatch = ctool::file::getFiles(testDirectory, "tmp_abc");
    EXPECT_GE(prefixMatch.size(), 3);
    EXPECT_NE(std::find(prefixMatch.begin(), prefixMatch.end(), f1), prefixMatch.end());
    EXPECT_NE(std::find(prefixMatch.begin(), prefixMatch.end(), f2), prefixMatch.end());
    EXPECT_NE(std::find(prefixMatch.begin(), prefixMatch.end(), f4), prefixMatch.end());

    // Match suffix only
    std::vector<std::string> suffixMatch = ctool::file::getFiles(testDirectory, "", ".test");
    EXPECT_GE(suffixMatch.size(), 3);
    EXPECT_NE(std::find(suffixMatch.begin(), suffixMatch.end(), f1), suffixMatch.end());
    EXPECT_NE(std::find(suffixMatch.begin(), suffixMatch.end(), f2), suffixMatch.end());
    EXPECT_NE(std::find(suffixMatch.begin(), suffixMatch.end(), f3), suffixMatch.end());

    // Match prefix and suffix
    std::vector<std::string> bothMatch = ctool::file::getFiles(testDirectory, "tmp_abc", ".test");
    EXPECT_GE(bothMatch.size(), 2);
    EXPECT_NE(std::find(bothMatch.begin(), bothMatch.end(), f1), bothMatch.end());
    EXPECT_NE(std::find(bothMatch.begin(), bothMatch.end(), f2), bothMatch.end());
    EXPECT_EQ(std::find(bothMatch.begin(), bothMatch.end(), f4), bothMatch.end());

    // No matches
    std::vector<std::string> noMatch = ctool::file::getFiles(testDirectory, "non_existent_prefix", ".test");
    EXPECT_TRUE(noMatch.empty());

    // Non-existent directory
    std::vector<std::string> nonExistentDir = ctool::file::getFiles("/non/existent/path/xyz");
    EXPECT_TRUE(nonExistentDir.empty());

    // Cleanup
    ctool::file::remove(f1);
    ctool::file::remove(f2);
    ctool::file::remove(f3);
    ctool::file::remove(f4);
}
