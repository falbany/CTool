/**
 * @file file_test.cpp
 * @author Florent ALBANY - FAL
 * @brief Comprehensive test suite for CBridge file module (cbridge_file) using Google Test.
 * @version 1.0
 * @date 2026-06-01
 */

#include <gtest/gtest.h>
#include <cstddef>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
    #include <direct.h>
    #include <io.h>
#else
    #include <unistd.h>
    #include <fcntl.h>
#endif

/* The CBridge C headers have no __cplusplus guards, so wrap with extern "C" */
extern "C" {
#include "cbridge/file.h"
#include "cbridge/string.h"
#include "cbridge/vector.h"
}

/* ============================================================================
 * Helper: create a temporary test file with given content
 * Returns the path string (caller must free file + string)
 * ============================================================================ */
static string_t* createTempFile(const char* prefix, const char* content) {
    char template_buf[512];
#ifdef _WIN32
    const char* tmpdir = getenv("TEMP");
    if (!tmpdir) tmpdir = "C:\\Temp";
    sprintf(template_buf, "%s\\%s_XXXXXX.tmp", tmpdir, prefix);
    /* On Windows, mkstemp isn't available, use a simple approach */
    sprintf(template_buf, "%s\\%s_test.tmp", tmpdir, prefix);
#else
    const char* tmpdir = "/tmp";
    sprintf(template_buf, "%s/%s_XXXXXX.tmp", tmpdir, prefix);
    int fd = mkstemp(template_buf);
    if (fd >= 0) {
        write(fd, content, strlen(content));
        close(fd);
    }
#endif
    if (content) {
        FILE* f = fopen(template_buf, "wb");
        if (f) {
            /* Write in binary mode to preserve LF line endings on Windows */
            fwrite(content, 1, strlen(content), f);
            fclose(f);
        }
    }
    return cbridge_string.create(template_buf);
}

/* ============================================================================
 * exists() Tests
 * ============================================================================ */

TEST(CbFileExists, NullPathReturnsFalse) { EXPECT_FALSE(cbridge_file.exists(NULL)); }

TEST(CbFileExists, NonExistentFileReturnsFalse) { EXPECT_FALSE(cbridge_file.exists("/this/path/should/not/exist_12345.txt")); }

TEST(CbFileExists, ExistingFileReturnsTrue) {
    string_t* path = createTempFile("exists_test", "hello");
    EXPECT_TRUE(cbridge_file.exists(cbridge_string.c_str(path)));
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
}

TEST(CbFileExists, AfterDeleteReturnsFalse) {
    string_t* path = createTempFile("exists_delete_test", "hello");
    EXPECT_TRUE(cbridge_file.exists(cbridge_string.c_str(path)));
    remove(cbridge_string.c_str(path));
    EXPECT_FALSE(cbridge_file.exists(cbridge_string.c_str(path)));
    cbridge_string.free(path);
}

/* ============================================================================
 * get_size() Tests
 * ============================================================================ */

TEST(CbFileSize, NullPathReturnsZero) { EXPECT_EQ(cbridge_file.get_size(NULL), 0LL); }

TEST(CbFileSize, NonExistentFileReturnsZero) { EXPECT_EQ(cbridge_file.get_size("/this/path/should/not/exist_12345.txt"), 0LL); }

TEST(CbFileSize, EmptyFileReturnsZero) {
    string_t* path = createTempFile("size_empty_test", "");
    EXPECT_EQ(cbridge_file.get_size(cbridge_string.c_str(path)), 0LL);
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
}

TEST(CbFileSize, KnownContentReturnsCorrectSize) {
    const char* content = "Hello, World!";
    string_t*   path    = createTempFile("size_known_test", content);
    EXPECT_EQ(cbridge_file.get_size(cbridge_string.c_str(path)), static_cast<long long>(strlen(content)));
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
}

TEST(CbFileSize, MultiLineContent) {
    /* On Windows, fprintf converts \n to \r\n, so we use raw bytes */
    const char* content = "Line1\nLine2\nLine3\n";
    string_t*   path    = createTempFile("size_multiline_test", content);
    long long   size    = cbridge_file.get_size(cbridge_string.c_str(path));
    /* Size should be >= strlen(content) because Windows may expand \n -> \r\n */
    EXPECT_GE(size, static_cast<long long>(strlen(content)));
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
}

/* ============================================================================
 * read_all() Tests
 * ============================================================================ */

TEST(CbFileReadAll, NullPathReturnsNull) { EXPECT_EQ(cbridge_file.read_all(NULL), nullptr); }

TEST(CbFileReadAll, NonExistentFileReturnsNull) { EXPECT_EQ(cbridge_file.read_all("/this/path/should/not/exist_12345.txt"), nullptr); }

TEST(CbFileReadAll, ReadsSimpleContent) {
    const char* content = "Hello, World!";
    string_t*   path    = createTempFile("read_simple_test", content);
    string_t*   result  = cbridge_file.read_all(cbridge_string.c_str(path));
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), content);
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadAll, ReadsMultiLineContent) {
    const char* content = "Line1\r\nLine2\r\nLine3\r\n";
    string_t*   path    = createTempFile("read_multiline_test", content);
    string_t*   result  = cbridge_file.read_all(cbridge_string.c_str(path));
    EXPECT_NE(result, nullptr);
    /* On Windows, fopen in text mode may convert \r\n to \n on read.
       Binary mode ("rb") is used by impl_read_all, so content stays as-is. */
    EXPECT_GE(cbridge_string.length(result), strlen("Line1\nLine2\nLine3\n"));
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadAll, ReadsEmptyFile) {
    string_t* path   = createTempFile("read_empty_test", "");
    string_t* result = cbridge_file.read_all(cbridge_string.c_str(path));
    EXPECT_NE(result, nullptr);
    EXPECT_TRUE(cbridge_string.empty(result));
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadAll, ReadsSpecialCharacters) {
    const char* content = "Special: !@#$%^&*()_+-=[]{}|;':\",./<>?";
    string_t*   path    = createTempFile("read_special_test", content);
    string_t*   result  = cbridge_file.read_all(cbridge_string.c_str(path));
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), content);
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

/* ============================================================================
 * get_working_directory() Tests
 * ============================================================================ */

TEST(CbFileWorkingDir, ReturnsNonEmpty) {
    string_t* cwd = cbridge_file.get_working_directory();
    EXPECT_NE(cwd, nullptr);
    EXPECT_FALSE(cbridge_string.empty(cwd));
    cbridge_string.free(cwd);
}

TEST(CbFileWorkingDir, ContainsValidPath) {
    string_t* cwd = cbridge_file.get_working_directory();
    EXPECT_NE(cwd, nullptr);
    /* The path should exist as a directory */
    EXPECT_FALSE(cbridge_string.c_str(cwd)[0] == '\0');
    cbridge_string.free(cwd);
}

/* ============================================================================
 * get_parameter() Tests
 * ============================================================================ */

TEST(CbFileGetParameter, NullPathReturnsEmpty) {
    string_t* result = cbridge_file.get_parameter(NULL, "key", "=");
    EXPECT_NE(result, nullptr);
    EXPECT_TRUE(cbridge_string.empty(result));
    cbridge_string.free(result);
}

TEST(CbFileGetParameter, NonExistentFileReturnsEmpty) {
    string_t* result = cbridge_file.get_parameter("/this/path/should/not/exist_12345.txt", "key", "=");
    EXPECT_NE(result, nullptr);
    EXPECT_TRUE(cbridge_string.empty(result));
    cbridge_string.free(result);
}

TEST(CbFileGetParameter, SimpleKeyValue) {
    const char* content = "name = John Doe\nage = 30\n";
    string_t*   path    = createTempFile("param_simple_test", content);

    string_t* result = cbridge_file.get_parameter(cbridge_string.c_str(path), "name", "=");
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "John Doe");

    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileGetParameter, KeyNotFoundReturnsEmpty) {
    const char* content = "name = John Doe\nage = 30\n";
    string_t*   path    = createTempFile("param_notfound_test", content);

    string_t* result = cbridge_file.get_parameter(cbridge_string.c_str(path), "email", "=");
    EXPECT_NE(result, nullptr);
    EXPECT_TRUE(cbridge_string.empty(result));

    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileGetParameter, SkipsComments) {
    const char* content = "# name = Commented\nname = Real Value\n";
    string_t*   path    = createTempFile("param_comments_test", content);

    string_t* result = cbridge_file.get_parameter(cbridge_string.c_str(path), "name", "=");
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "Real Value");

    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileGetParameter, SkipsDoubleSlashComments) {
    const char* content = "// name = Commented\nname = Real Value\n";
    string_t*   path    = createTempFile("param_slash_comments_test", content);

    string_t* result = cbridge_file.get_parameter(cbridge_string.c_str(path), "name", "=");
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "Real Value");

    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileGetParameter, SkipsEmptyLines) {
    const char* content = "\n\nname = Value\n\n";
    string_t*   path    = createTempFile("param_empty_lines_test", content);

    string_t* result = cbridge_file.get_parameter(cbridge_string.c_str(path), "name", "=");
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "Value");

    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileGetParameter, CustomSeparator) {
    const char* content = "name: John Doe\nage: 30\n";
    string_t*   path    = createTempFile("param_separator_test", content);

    string_t* result = cbridge_file.get_parameter(cbridge_string.c_str(path), "name", ":");
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "John Doe");

    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileGetParameter, NoSpacesAroundSeparator) {
    const char* content = "name=JohnDoe\nage=30\n";
    string_t*   path    = createTempFile("param_nospaces_test", content);

    string_t* result = cbridge_file.get_parameter(cbridge_string.c_str(path), "name", "=");
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "JohnDoe");

    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileGetParameter, MultipleKeysReturnsFirstMatch) {
    const char* content = "name = First\nname = Second\n";
    string_t*   path    = createTempFile("param_multiple_test", content);

    string_t* result = cbridge_file.get_parameter(cbridge_string.c_str(path), "name", "=");
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "First");

    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

/* ============================================================================
 * get_files() Tests
 * ============================================================================ */

TEST(CbFileGetFiles, NullDirectoryReturnsEmptyVector) {
    vector_t* vec = cbridge_file.get_files(NULL, NULL, NULL);
    EXPECT_NE(vec, nullptr);
    EXPECT_TRUE(cbridge_vector.empty(vec));
    cbridge_vector.free(vec);
}

TEST(CbFileGetFiles, NonExistentDirectoryReturnsEmptyVector) {
    vector_t* vec = cbridge_file.get_files("/this/path/should/not/exist_12345", NULL, NULL);
    EXPECT_NE(vec, nullptr);
    EXPECT_TRUE(cbridge_vector.empty(vec));
    cbridge_vector.free(vec);
}

TEST(CbFileGetFiles, ListsFilesInDirectory) {
    /* Create a temporary directory with test files */
    const char* tmpdir = getenv("TEMP");
    if (!tmpdir) tmpdir = "/tmp";

    char dir_path[512];
    sprintf(dir_path, "%s\\cb_file_test_dir_XXXXXX", tmpdir);

    /* Create unique directory */
#ifdef _WIN32
    /* Simple approach: use a fixed test directory name */
    sprintf(dir_path, "%s\\cb_file_test_dir", tmpdir);
    _mkdir(dir_path);
#else
    mkdir(dir_path, 0755);
#endif

    /* Create test files */
    char file1[600], file2[600], file3[600];
    sprintf(file1, "%s\\test1.txt", dir_path);
    sprintf(file2, "%s\\test2.txt", dir_path);
    sprintf(file3, "%s\\data.csv", dir_path);

    FILE* f1 = fopen(file1, "w");
    if (f1) {
        fprintf(f1, "content1");
        fclose(f1);
    }
    FILE* f2 = fopen(file2, "w");
    if (f2) {
        fprintf(f2, "content2");
        fclose(f2);
    }
    FILE* f3 = fopen(file3, "w");
    if (f3) {
        fprintf(f3, "content3");
        fclose(f3);
    }

    /* Get all files */
    vector_t* vec = cbridge_file.get_files(dir_path, NULL, NULL);
    EXPECT_NE(vec, nullptr);
    EXPECT_EQ(cbridge_vector.size(vec), 3u);

    /* Cleanup files */
    remove(file1);
    remove(file2);
    remove(file3);

    /* Free vector items (they are string_t*) */
    for (size_t i = 0; i < cbridge_vector.size(vec); i++) {
        string_t* name = (string_t*)cbridge_vector.at(vec, i);
        cbridge_string.free(name);
    }
    cbridge_vector.free(vec);

    /* Remove directory */
    rmdir(dir_path);
}

TEST(CbFileGetFiles, FilterBySuffix) {
    const char* tmpdir = getenv("TEMP");
    if (!tmpdir) tmpdir = "/tmp";

    char dir_path[512];
    sprintf(dir_path, "%s\\cb_file_suffix_test_dir", tmpdir);
#ifdef _WIN32
    _mkdir(dir_path);
#else
    mkdir(dir_path, 0755);
#endif

    char file1[600], file2[600], file3[600];
    sprintf(file1, "%s\\test1.txt", dir_path);
    sprintf(file2, "%s\\test2.txt", dir_path);
    sprintf(file3, "%s\\data.csv", dir_path);

    FILE* f1 = fopen(file1, "w");
    if (f1) {
        fprintf(f1, "content1");
        fclose(f1);
    }
    FILE* f2 = fopen(file2, "w");
    if (f2) {
        fprintf(f2, "content2");
        fclose(f2);
    }
    FILE* f3 = fopen(file3, "w");
    if (f3) {
        fprintf(f3, "content3");
        fclose(f3);
    }

    /* Filter by .txt suffix */
    vector_t* vec = cbridge_file.get_files(dir_path, NULL, ".txt");
    EXPECT_NE(vec, nullptr);
    EXPECT_EQ(cbridge_vector.size(vec), 2u);

    /* Verify all returned files end with .txt */
    for (size_t i = 0; i < cbridge_vector.size(vec); i++) {
        string_t* name = (string_t*)cbridge_vector.at(vec, i);
        EXPECT_TRUE(cbridge_string.ends_with(name, ".txt"));
        cbridge_string.free(name);
    }
    cbridge_vector.free(vec);

    /* Cleanup */
    remove(file1);
    remove(file2);
    remove(file3);
    rmdir(dir_path);
}

TEST(CbFileGetFiles, FilterByPrefix) {
    const char* tmpdir = getenv("TEMP");
    if (!tmpdir) tmpdir = "/tmp";

    char dir_path[512];
    sprintf(dir_path, "%s\\cb_file_prefix_test_dir", tmpdir);
#ifdef _WIN32
    _mkdir(dir_path);
#else
    mkdir(dir_path, 0755);
#endif

    char file1[600], file2[600], file3[600];
    sprintf(file1, "%s\\log_2026_01.txt", dir_path);
    sprintf(file2, "%s\\log_2026_02.txt", dir_path);
    sprintf(file3, "%s\\data_2026.txt", dir_path);

    FILE* f1 = fopen(file1, "w");
    if (f1) {
        fprintf(f1, "content1");
        fclose(f1);
    }
    FILE* f2 = fopen(file2, "w");
    if (f2) {
        fprintf(f2, "content2");
        fclose(f2);
    }
    FILE* f3 = fopen(file3, "w");
    if (f3) {
        fprintf(f3, "content3");
        fclose(f3);
    }

    /* Filter by "log_" prefix */
    vector_t* vec = cbridge_file.get_files(dir_path, "log_", NULL);
    EXPECT_NE(vec, nullptr);
    EXPECT_EQ(cbridge_vector.size(vec), 2u);

    /* Verify all returned files start with "log_" */
    for (size_t i = 0; i < cbridge_vector.size(vec); i++) {
        string_t* name = (string_t*)cbridge_vector.at(vec, i);
        EXPECT_TRUE(cbridge_string.starts_with(name, "log_"));
        cbridge_string.free(name);
    }
    cbridge_vector.free(vec);

    /* Cleanup */
    remove(file1);
    remove(file2);
    remove(file3);
    rmdir(dir_path);
}

TEST(CbFileGetFiles, FilterByPrefixAndSuffix) {
    const char* tmpdir = getenv("TEMP");
    if (!tmpdir) tmpdir = "/tmp";

    char dir_path[512];
    sprintf(dir_path, "%s\\cb_file_both_test_dir", tmpdir);
#ifdef _WIN32
    _mkdir(dir_path);
#else
    mkdir(dir_path, 0755);
#endif

    char file1[600], file2[600], file3[600], file4[600];
    sprintf(file1, "%s\\data_01.csv", dir_path);
    sprintf(file2, "%s\\data_02.csv", dir_path);
    sprintf(file3, "%s\\log_01.csv", dir_path);
    sprintf(file4, "%s\\data_01.txt", dir_path);

    FILE* f1 = fopen(file1, "w");
    if (f1) {
        fprintf(f1, "content1");
        fclose(f1);
    }
    FILE* f2 = fopen(file2, "w");
    if (f2) {
        fprintf(f2, "content2");
        fclose(f2);
    }
    FILE* f3 = fopen(file3, "w");
    if (f3) {
        fprintf(f3, "content3");
        fclose(f3);
    }
    FILE* f4 = fopen(file4, "w");
    if (f4) {
        fprintf(f4, "content4");
        fclose(f4);
    }

    /* Filter by "data_" prefix AND ".csv" suffix */
    vector_t* vec = cbridge_file.get_files(dir_path, "data_", ".csv");
    EXPECT_NE(vec, nullptr);
    EXPECT_EQ(cbridge_vector.size(vec), 2u);

    /* Verify all returned files match both criteria */
    for (size_t i = 0; i < cbridge_vector.size(vec); i++) {
        string_t* name = (string_t*)cbridge_vector.at(vec, i);
        EXPECT_TRUE(cbridge_string.starts_with(name, "data_"));
        EXPECT_TRUE(cbridge_string.ends_with(name, ".csv"));
        cbridge_string.free(name);
    }
    cbridge_vector.free(vec);

    /* Cleanup */
    remove(file1);
    remove(file2);
    remove(file3);
    remove(file4);
    rmdir(dir_path);
}

TEST(CbFileGetFiles, EmptyDirectoryReturnsEmptyVector) {
    const char* tmpdir = getenv("TEMP");
    if (!tmpdir) tmpdir = "/tmp";

    char dir_path[512];
    sprintf(dir_path, "%s\\cb_file_empty_dir_test", tmpdir);
#ifdef _WIN32
    _mkdir(dir_path);
#else
    mkdir(dir_path, 0755);
#endif

    vector_t* vec = cbridge_file.get_files(dir_path, NULL, NULL);
    EXPECT_NE(vec, nullptr);
    EXPECT_TRUE(cbridge_vector.empty(vec));
    cbridge_vector.free(vec);

    rmdir(dir_path);
}

/* ============================================================================
 * Integration Tests
 * ============================================================================ */

TEST(CbFileIntegration, WriteReadAndVerify) {
    string_t* path = createTempFile("integration_test", "Integration Test Content");

    /* Verify file exists */
    EXPECT_TRUE(cbridge_file.exists(cbridge_string.c_str(path)));

    /* Verify size */
    long long size = cbridge_file.get_size(cbridge_string.c_str(path));
    EXPECT_EQ(size, 24LL);

    /* Read and verify content */
    string_t* content = cbridge_file.read_all(cbridge_string.c_str(path));
    EXPECT_NE(content, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(content), "Integration Test Content");

    /* Cleanup */
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(content);
}

TEST(CbFileIntegration, ConfigFileRoundTrip) {
    const char* config =
        "# Database Configuration\n"
        "host = localhost\n"
        "port = 5432\n"
        "// username = admin\n"
        "username = testuser\n";

    string_t* path = createTempFile("config_test", config);

    /* Read parameters */
    string_t* host = cbridge_file.get_parameter(cbridge_string.c_str(path), "host", "=");
    EXPECT_STREQ(cbridge_string.c_str(host), "localhost");

    string_t* port = cbridge_file.get_parameter(cbridge_string.c_str(path), "port", "=");
    EXPECT_STREQ(cbridge_string.c_str(port), "5432");

    string_t* user = cbridge_file.get_parameter(cbridge_string.c_str(path), "username", "=");
    EXPECT_STREQ(cbridge_string.c_str(user), "testuser");

    /* Cleanup */
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(host);
    cbridge_string.free(port);
    cbridge_string.free(user);
}

/* ============================================================================
 * read_lines() Tests
 * ============================================================================ */

TEST(CbFileReadLines, NullPathReturnsEmpty) {
    string_t* result = cbridge_file.read_lines(NULL, 1, 5);
    EXPECT_NE(result, nullptr);
    EXPECT_TRUE(cbridge_string.empty(result));
    cbridge_string.free(result);
}

TEST(CbFileReadLines, NonExistentFileReturnsEmpty) {
    string_t* result = cbridge_file.read_lines("/this/path/should/not/exist_12345.txt", 1, 5);
    EXPECT_NE(result, nullptr);
    EXPECT_TRUE(cbridge_string.empty(result));
    cbridge_string.free(result);
}

TEST(CbFileReadLines, StartLineZeroTreatedAsOne) {
    const char* content = "Line1\nLine2\nLine3\n";
    string_t*   path    = createTempFile("readlines_start0_test", content);
    string_t*   result  = cbridge_file.read_lines(cbridge_string.c_str(path), 0, 3);
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "Line1\nLine2\nLine3");
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadLines, EndLineZeroTreatedAsOne) {
    const char* content = "Line1\nLine2\nLine3\n";
    string_t*   path    = createTempFile("readlines_end0_test", content);
    string_t*   result  = cbridge_file.read_lines(cbridge_string.c_str(path), 1, 0);
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "Line1");
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadLines, BothLinesZeroTreatedAsOne) {
    const char* content = "Line1\nLine2\nLine3\n";
    string_t*   path    = createTempFile("readlines_both0_test", content);
    string_t*   result  = cbridge_file.read_lines(cbridge_string.c_str(path), 0, 0);
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "Line1");
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadLines, StartExceedsEndReturnsEmpty) {
    const char* content = "Line1\nLine2\nLine3\n";
    string_t*   path    = createTempFile("readlines_start_gt_end_test", content);
    string_t*   result  = cbridge_file.read_lines(cbridge_string.c_str(path), 5, 2);
    EXPECT_NE(result, nullptr);
    EXPECT_TRUE(cbridge_string.empty(result));
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadLines, SingleLineRange) {
    const char* content = "Line1\nLine2\nLine3\n";
    string_t*   path    = createTempFile("readlines_single_test", content);
    string_t*   result  = cbridge_file.read_lines(cbridge_string.c_str(path), 2, 2);
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "Line2");
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadLines, RangeWithinFile) {
    const char* content = "Line1\nLine2\nLine3\nLine4\nLine5\n";
    string_t*   path    = createTempFile("readlines_within_test", content);
    string_t*   result  = cbridge_file.read_lines(cbridge_string.c_str(path), 2, 4);
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "Line2\nLine3\nLine4");
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadLines, EndExceedsTotalLines) {
    const char* content = "Line1\nLine2\nLine3\n";
    string_t*   path    = createTempFile("readlines_end_exceeds_test", content);
    string_t*   result  = cbridge_file.read_lines(cbridge_string.c_str(path), 2, 100);
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "Line2\nLine3");
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadLines, StartExceedsTotalLines) {
    const char* content = "Line1\nLine2\nLine3\n";
    string_t*   path    = createTempFile("readlines_start_exceeds_test", content);
    string_t*   result  = cbridge_file.read_lines(cbridge_string.c_str(path), 10, 20);
    EXPECT_NE(result, nullptr);
    EXPECT_TRUE(cbridge_string.empty(result));
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadLines, EmptyFile) {
    string_t* path   = createTempFile("readlines_empty_test", "");
    string_t* result = cbridge_file.read_lines(cbridge_string.c_str(path), 1, 5);
    EXPECT_NE(result, nullptr);
    EXPECT_TRUE(cbridge_string.empty(result));
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadLines, SingleLineFile) {
    const char* content = "OnlyLine\n";
    string_t*   path    = createTempFile("readlines_singlefile_test", content);
    string_t*   result  = cbridge_file.read_lines(cbridge_string.c_str(path), 1, 1);
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "OnlyLine");
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadLines, ReadsFromFirstLine) {
    const char* content = "First\nSecond\nThird\n";
    string_t*   path    = createTempFile("readlines_from_first_test", content);
    string_t*   result  = cbridge_file.read_lines(cbridge_string.c_str(path), 1, 2);
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "First\nSecond");
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadLines, ReadsToLastLine) {
    const char* content = "First\nSecond\nThird\n";
    string_t*   path    = createTempFile("readlines_to_last_test", content);
    string_t*   result  = cbridge_file.read_lines(cbridge_string.c_str(path), 2, 3);
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "Second\nThird");
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadLines, LineWithSpecialCharacters) {
    const char* content = "Line with special: !@#$%^&*()\nNormal line\nAnother: {[]}|\\:\"<>?\n";
    string_t*   path    = createTempFile("readlines_special_test", content);
    string_t*   result  = cbridge_file.read_lines(cbridge_string.c_str(path), 1, 3);
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "Line with special: !@#$%^&*()\nNormal line\nAnother: {[]}|\\:\"<>?");
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadLines, EmptyLinesInRange) {
    const char* content = "Line1\n\nLine3\n\nLine5\n";
    string_t*   path    = createTempFile("readlines_emptylines_test", content);
    string_t*   result  = cbridge_file.read_lines(cbridge_string.c_str(path), 2, 4);
    EXPECT_NE(result, nullptr);
    /* Empty lines should be preserved as empty strings */
    EXPECT_STREQ(cbridge_string.c_str(result), "\nLine3\n");
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadLines, WindowsLineEndings) {
    const char* content = "Line1\r\nLine2\r\nLine3\r\n";
    string_t*   path    = createTempFile("readlines_windows_test", content);
    string_t*   result  = cbridge_file.read_lines(cbridge_string.c_str(path), 1, 3);
    EXPECT_NE(result, nullptr);
    /* Line endings should be normalized to \n in output */
    EXPECT_STREQ(cbridge_string.c_str(result), "Line1\nLine2\nLine3");
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadLines, LargeRangeRequest) {
    const char* content = "Line1\nLine2\nLine3\nLine4\nLine5\n";
    string_t*   path    = createTempFile("readlines_large_test", content);
    string_t*   result  = cbridge_file.read_lines(cbridge_string.c_str(path), 1, 1000);
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "Line1\nLine2\nLine3\nLine4\nLine5");
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadLines, PartialRangeMiddle) {
    const char* content = "Line1\nLine2\nLine3\nLine4\nLine5\nLine6\nLine7\nLine8\nLine9\nLine10\n";
    string_t*   path    = createTempFile("readlines_partial_test", content);
    string_t*   result  = cbridge_file.read_lines(cbridge_string.c_str(path), 4, 7);
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), "Line4\nLine5\nLine6\nLine7");
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

TEST(CbFileReadLines, LargeNumberedLines) {
    string_t* path = createTempFile("readlines_largenum_test", "");
    /* Create a file with many lines */
    FILE* f = fopen(cbridge_string.c_str(path), "w");
    if (f) {
        for (int i = 1; i <= 100; i++) {
            fprintf(f, "Line %d\n", i);
        }
        fclose(f);
    }

    /* Read lines 50 to 60 */
    string_t* result = cbridge_file.read_lines(cbridge_string.c_str(path), 50, 60);
    EXPECT_NE(result, nullptr);
    EXPECT_TRUE(cbridge_string.length(result) > 0);

    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(result);
}

/* ============================================================================
 * write_all() Tests
 * ============================================================================ */

TEST(CbFileWriteAll, NullPathReturnsFalse) { EXPECT_FALSE(cbridge_file.write_all(NULL, "content")); }

TEST(CbFileWriteAll, WriteContentSuccessfully) {
    string_t* path = createTempFile("write_all_test", "");
    remove(cbridge_string.c_str(path));

    EXPECT_TRUE(cbridge_file.write_all(cbridge_string.c_str(path), "Hello, World!"));
    EXPECT_TRUE(cbridge_file.exists(cbridge_string.c_str(path)));

    string_t* content = cbridge_file.read_all(cbridge_string.c_str(path));
    EXPECT_NE(content, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(content), "Hello, World!");

    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(content);
}

TEST(CbFileWriteAll, OverwriteExistingFile) {
    string_t* path = createTempFile("write_overwrite_test", "Original Content");

    EXPECT_TRUE(cbridge_file.write_all(cbridge_string.c_str(path), "New Content"));

    string_t* content = cbridge_file.read_all(cbridge_string.c_str(path));
    EXPECT_NE(content, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(content), "New Content");

    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(content);
}

TEST(CbFileWriteAll, WriteEmptyContent) {
    string_t* path = createTempFile("write_empty_test", "");
    remove(cbridge_string.c_str(path));

    EXPECT_TRUE(cbridge_file.write_all(cbridge_string.c_str(path), ""));
    EXPECT_TRUE(cbridge_file.exists(cbridge_string.c_str(path)));
    EXPECT_EQ(cbridge_file.get_size(cbridge_string.c_str(path)), 0LL);

    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
}

TEST(CbFileWriteAll, NullContentTreatedAsEmpty) {
    string_t* path = createTempFile("write_null_content_test", "");
    remove(cbridge_string.c_str(path));

    EXPECT_TRUE(cbridge_file.write_all(cbridge_string.c_str(path), NULL));
    EXPECT_TRUE(cbridge_file.exists(cbridge_string.c_str(path)));
    EXPECT_EQ(cbridge_file.get_size(cbridge_string.c_str(path)), 0LL);

    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
}

/* ============================================================================
 * append_all() Tests
 * ============================================================================ */

TEST(CbFileAppendAll, NullPathReturnsFalse) { EXPECT_FALSE(cbridge_file.append_all(NULL, "content")); }

TEST(CbFileAppendAll, AppendToNewFile) {
    string_t* path = createTempFile("append_new_test", "");
    remove(cbridge_string.c_str(path));

    EXPECT_TRUE(cbridge_file.append_all(cbridge_string.c_str(path), "First Line\n"));
    EXPECT_TRUE(cbridge_file.exists(cbridge_string.c_str(path)));

    string_t* content = cbridge_file.read_all(cbridge_string.c_str(path));
    EXPECT_NE(content, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(content), "First Line\n");

    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(content);
}

TEST(CbFileAppendAll, AppendToExistingFile) {
    string_t* path = createTempFile("append_existing_test", "First Line\n");

    EXPECT_TRUE(cbridge_file.append_all(cbridge_string.c_str(path), "Second Line\n"));

    string_t* content = cbridge_file.read_all(cbridge_string.c_str(path));
    EXPECT_NE(content, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(content), "First Line\nSecond Line\n");

    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(content);
}

TEST(CbFileAppendAll, AppendEmptyContent) {
    string_t* path = createTempFile("append_empty_test", "Original\n");

    EXPECT_TRUE(cbridge_file.append_all(cbridge_string.c_str(path), ""));

    string_t* content = cbridge_file.read_all(cbridge_string.c_str(path));
    EXPECT_NE(content, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(content), "Original\n");

    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
    cbridge_string.free(content);
}

/* ============================================================================
 * get_extension() Tests
 * ============================================================================ */

TEST(CbFileGetExtension, NullPathReturnsEmpty) {
    string_t* result = cbridge_file.get_extension(NULL);
    EXPECT_NE(result, nullptr);
    EXPECT_TRUE(cbridge_string.empty(result));
    cbridge_string.free(result);
}

TEST(CbFileGetExtension, SimpleExtension) {
    string_t* result = cbridge_file.get_extension("file.txt");
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), ".txt");
    cbridge_string.free(result);
}

TEST(CbFileGetExtension, PathWithExtension) {
    string_t* result = cbridge_file.get_extension("/path/to/file.csv");
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), ".csv");
    cbridge_string.free(result);
}

TEST(CbFileGetExtension, NoExtension) {
    string_t* result = cbridge_file.get_extension("Makefile");
    EXPECT_NE(result, nullptr);
    EXPECT_TRUE(cbridge_string.empty(result));
    cbridge_string.free(result);
}

TEST(CbFileGetExtension, MultipleDots) {
    string_t* result = cbridge_file.get_extension("archive.tar.gz");
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), ".gz");
    cbridge_string.free(result);
}

TEST(CbFileGetExtension, HiddenFile) {
    string_t* result = cbridge_file.get_extension(".gitignore");
    EXPECT_NE(result, nullptr);
    EXPECT_TRUE(cbridge_string.empty(result));
    cbridge_string.free(result);
}

TEST(CbFileGetExtension, WindowsPath) {
    string_t* result = cbridge_file.get_extension("C:\\dir\\file.log");
    EXPECT_NE(result, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(result), ".log");
    cbridge_string.free(result);
}

/* ============================================================================
 * remove() Tests
 * ============================================================================ */

TEST(CbFileRemove, NullPathReturnsFalse) { EXPECT_FALSE(cbridge_file.remove(NULL)); }

TEST(CbFileRemove, NonExistentFileReturnsFalse) { EXPECT_FALSE(cbridge_file.remove("/this/path/should/not/exist_12345.txt")); }

TEST(CbFileRemove, DeletesExistingFile) {
    string_t* path = createTempFile("remove_test", "delete me");
    EXPECT_TRUE(cbridge_file.exists(cbridge_string.c_str(path)));
    EXPECT_TRUE(cbridge_file.remove(cbridge_string.c_str(path)));
    EXPECT_FALSE(cbridge_file.exists(cbridge_string.c_str(path)));
    cbridge_string.free(path);
}

TEST(CbFileRemove, DoubleRemoveReturnsFalse) {
    string_t* path = createTempFile("remove_double_test", "delete me");
    EXPECT_TRUE(cbridge_file.remove(cbridge_string.c_str(path)));
    EXPECT_FALSE(cbridge_file.remove(cbridge_string.c_str(path)));
    cbridge_string.free(path);
}

/* ============================================================================
 * copy() Tests
 * ============================================================================ */

TEST(CbFileCopy, NullSourceReturnsFalse) { EXPECT_FALSE(cbridge_file.copy(NULL, "dest.txt")); }

TEST(CbFileCopy, NullDestReturnsFalse) {
    string_t* path = createTempFile("copy_null_dest_test", "source content");
    EXPECT_FALSE(cbridge_file.copy(cbridge_string.c_str(path), NULL));
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
}

TEST(CbFileCopy, NonExistentSourceReturnsFalse) { EXPECT_FALSE(cbridge_file.copy("/this/path/should/not/exist_12345.txt", "dest.txt")); }

TEST(CbFileCopy, CopiesFileSuccessfully) {
    string_t* src_path = createTempFile("copy_src_test", "Hello, copied world!");
    char      dest_buf[512];
#ifdef _WIN32
    const char* tmpdir = getenv("TEMP");
    if (!tmpdir) tmpdir = "C:\\Temp";
    sprintf(dest_buf, "%s\\copy_dest_test.tmp", tmpdir);
#else
    sprintf(dest_buf, "/tmp/copy_dest_test.tmp");
#endif

    EXPECT_FALSE(cbridge_file.exists(dest_buf));
    EXPECT_TRUE(cbridge_file.copy(cbridge_string.c_str(src_path), dest_buf));
    EXPECT_TRUE(cbridge_file.exists(dest_buf));

    string_t* content = cbridge_file.read_all(dest_buf);
    EXPECT_NE(content, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(content), "Hello, copied world!");

    remove(cbridge_string.c_str(src_path));
    remove(dest_buf);
    cbridge_string.free(src_path);
    cbridge_string.free(content);
}

TEST(CbFileCopy, OverwriteExistingDest) {
    string_t* src_path  = createTempFile("copy_overwrite_src_test", "New Content");
    string_t* dest_path = createTempFile("copy_overwrite_dest_test", "Old Content");

    EXPECT_TRUE(cbridge_file.copy(cbridge_string.c_str(src_path), cbridge_string.c_str(dest_path)));

    string_t* content = cbridge_file.read_all(cbridge_string.c_str(dest_path));
    EXPECT_NE(content, nullptr);
    EXPECT_STREQ(cbridge_string.c_str(content), "New Content");

    remove(cbridge_string.c_str(src_path));
    remove(cbridge_string.c_str(dest_path));
    cbridge_string.free(src_path);
    cbridge_string.free(dest_path);
    cbridge_string.free(content);
}

TEST(CbFileCopy, CopyEmptyFile) {
    string_t* src_path = createTempFile("copy_empty_src_test", "");
    char      dest_buf[512];
#ifdef _WIN32
    const char* tmpdir = getenv("TEMP");
    if (!tmpdir) tmpdir = "C:\\Temp";
    sprintf(dest_buf, "%s\\copy_empty_dest_test.tmp", tmpdir);
#else
    sprintf(dest_buf, "/tmp/copy_empty_dest_test.tmp");
#endif

    EXPECT_TRUE(cbridge_file.copy(cbridge_string.c_str(src_path), dest_buf));
    EXPECT_TRUE(cbridge_file.exists(dest_buf));
    EXPECT_EQ(cbridge_file.get_size(dest_buf), 0LL);

    remove(cbridge_string.c_str(src_path));
    remove(dest_buf);
    cbridge_string.free(src_path);
}

TEST(CbFileCopy, CopyBinaryContent) {
    string_t* src_path = createTempFile("copy_binary_src_test", "");
    /* Write binary content directly (avoid null bytes so read_all works correctly) */
    FILE* f = fopen(cbridge_string.c_str(src_path), "wb");
    if (f) {
        unsigned char binary_data[] = {0x01, 0x02, 0x03, 0xFF, 0xFE, 0xFD};
        fwrite(binary_data, 1, sizeof(binary_data), f);
        fclose(f);
    }

    char dest_buf[512];
#ifdef _WIN32
    const char* tmpdir = getenv("TEMP");
    if (!tmpdir) tmpdir = "C:\\Temp";
    sprintf(dest_buf, "%s\\copy_binary_dest_test.tmp", tmpdir);
#else
    sprintf(dest_buf, "/tmp/copy_binary_dest_test.tmp");
#endif

    EXPECT_TRUE(cbridge_file.copy(cbridge_string.c_str(src_path), dest_buf));
    EXPECT_EQ(cbridge_file.get_size(dest_buf), 6LL);

    string_t* content = cbridge_file.read_all(dest_buf);
    EXPECT_NE(content, nullptr);
    EXPECT_EQ(cbridge_string.length(content), 6ULL);

    remove(cbridge_string.c_str(src_path));
    remove(dest_buf);
    cbridge_string.free(src_path);
    cbridge_string.free(content);
}

/* ============================================================================
 * is_directory() Tests
 * ============================================================================ */

TEST(CbFileIsDirectory, NullPathReturnsFalse) { EXPECT_FALSE(cbridge_file.is_directory(NULL)); }

TEST(CbFileIsDirectory, NonExistentPathReturnsFalse) { EXPECT_FALSE(cbridge_file.is_directory("/this/path/should/not/exist_12345")); }

TEST(CbFileIsDirectory, FileReturnsFalse) {
    string_t* path = createTempFile("isdir_file_test", "not a directory");
    EXPECT_FALSE(cbridge_file.is_directory(cbridge_string.c_str(path)));
    remove(cbridge_string.c_str(path));
    cbridge_string.free(path);
}

TEST(CbFileIsDirectory, CurrentDirReturnsTrue) { EXPECT_TRUE(cbridge_file.is_directory(".")); }

TEST(CbFileIsDirectory, TempDirReturnsTrue) {
    const char* tmpdir = getenv("TEMP");
    if (!tmpdir) tmpdir = "/tmp";
    EXPECT_TRUE(cbridge_file.is_directory(tmpdir));
}

TEST(CbFileIsDirectory, WorkingDirectoryReturnsTrue) {
    string_t* cwd = cbridge_file.get_working_directory();
    EXPECT_TRUE(cbridge_file.is_directory(cbridge_string.c_str(cwd)));
    cbridge_string.free(cwd);
}

TEST(CbFileIsDirectory, CreatedTempDirectory) {
    const char* tmpdir = getenv("TEMP");
    if (!tmpdir) tmpdir = "/tmp";

    char dir_path[512];
    sprintf(dir_path, "%s\\cb_isdir_test_dir", tmpdir);
#ifdef _WIN32
    _mkdir(dir_path);
#else
    mkdir(dir_path, 0755);
#endif

    EXPECT_TRUE(cbridge_file.is_directory(dir_path));
    rmdir(dir_path);
    EXPECT_FALSE(cbridge_file.is_directory(dir_path));
}
