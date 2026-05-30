/**
 * @file ini_test.cpp
 * @author Florent ALBANY - FAL
 * @brief Comprehensive test suite for CTool INI module (ctool_ini) using Google Test.
 * @version 1.0
 * @date 2026-05-29
 */

#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <algorithm>

#include "ctool/ini.hpp"

namespace fs = std::filesystem;

/* ============================================================================
 * Test Fixture for INI Tests
 * ============================================================================ */

class CtIniTest : public ::testing::Test {
  protected:
    // Create a temporary directory for test files
    static std::string test_dir;
    static std::string test_file;

    static void SetUpTestSuite() {
        test_dir = fs::temp_directory_path().string() + "/ctool_ini_tests";
        fs::create_directories(test_dir);
    }

    static void TearDownTestSuite() {
        // Clean up temporary files
        if (fs::exists(test_dir)) {
            fs::remove_all(test_dir);
        }
    }

    // Generate unique test file paths
    std::string getTempFilePath(const std::string& filename) { return test_dir + "/" + filename; }

    // Helper to create test INI files
    void createTestFile(const std::string& filename, const std::string& content) {
        std::ofstream file(getTempFilePath(filename));
        file << content;
        file.close();
    }
};

std::string CtIniTest::test_dir;
std::string CtIniTest::test_file;

/* ============================================================================
 * Basic Loading and Parsing Tests
 * ============================================================================ */

TEST_F(CtIniTest, LoadSimpleIniFile) {
    std::string content = R"([database]
host = localhost
port = 5432

[application]
name = MyApp
version = 1.0.0
)";
    createTestFile("simple.ini", content);

    ctool::ini::Profile profile;
    bool                result = profile.load(getTempFilePath("simple.ini"));

    EXPECT_TRUE(result);
    EXPECT_TRUE(profile.hasSection("database"));
    EXPECT_TRUE(profile.hasSection("application"));
}

TEST_F(CtIniTest, LoadEmptyFile) {
    createTestFile("empty.ini", "");

    ctool::ini::Profile profile;
    bool                result = profile.load(getTempFilePath("empty.ini"));

    EXPECT_TRUE(result);
}

TEST_F(CtIniTest, LoadNonexistentFile) {
    ctool::ini::Profile profile;
    bool                result = profile.load(getTempFilePath("nonexistent.ini"));

    EXPECT_FALSE(result);
}

TEST_F(CtIniTest, LoadFileWithDefaultSection) {
    std::string content = R"(key1 = value1
key2 = value2

[section1]
key3 = value3
)";
    createTestFile("default_section.ini", content);

    ctool::ini::Profile profile;
    bool                result = profile.load(getTempFilePath("default_section.ini"));

    EXPECT_TRUE(result);
    EXPECT_TRUE(profile.hasSection("default"));
}

/* ============================================================================
 * Get and Set Tests
 * ============================================================================ */

TEST_F(CtIniTest, GetExistingValue) {
    std::string content = R"([database]
host = localhost
port = 5432
)";
    createTestFile("get_test.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("get_test.ini"));

    std::string host = profile.get("database", "host");
    std::string port = profile.get("database", "port");

    EXPECT_EQ(host, "localhost");
    EXPECT_EQ(port, "5432");
}

TEST_F(CtIniTest, GetMissingKey) {
    std::string content = R"([database]
host = localhost
)";
    createTestFile("missing_key.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("missing_key.ini"));

    std::string result = profile.get("database", "missing_key");
    EXPECT_EQ(result, "");
}

TEST_F(CtIniTest, GetMissingKeyWithDefault) {
    std::string content = R"([database]
host = localhost
)";
    createTestFile("missing_with_default.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("missing_with_default.ini"));

    std::string result = profile.get("database", "port", "5432");
    EXPECT_EQ(result, "5432");
}

TEST_F(CtIniTest, GetFromMissingSection) {
    std::string content = R"([database]
host = localhost
)";
    createTestFile("missing_section.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("missing_section.ini"));

    std::string result = profile.get("missing_section", "key");
    EXPECT_EQ(result, "");
}

TEST_F(CtIniTest, GetFromMissingSectionWithDefault) {
    std::string content = R"([database]
host = localhost
)";
    createTestFile("missing_section_default.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("missing_section_default.ini"));

    std::string result = profile.get("missing_section", "key", "default_value");
    EXPECT_EQ(result, "default_value");
}

TEST_F(CtIniTest, SetNewValue) {
    ctool::ini::Profile profile;
    profile.set("database", "host", "localhost");

    std::string result = profile.get("database", "host");
    EXPECT_EQ(result, "localhost");
}

TEST_F(CtIniTest, SetOverwriteExistingValue) {
    std::string content = R"([database]
host = oldhost
)";
    createTestFile("overwrite.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("overwrite.ini"));

    profile.set("database", "host", "newhost");
    std::string result = profile.get("database", "host");

    EXPECT_EQ(result, "newhost");
}

TEST_F(CtIniTest, SetCreateNewSection) {
    ctool::ini::Profile profile;
    profile.set("new_section", "key", "value");

    EXPECT_TRUE(profile.hasSection("new_section"));
    EXPECT_EQ(profile.get("new_section", "key"), "value");
}

/* ============================================================================
 * Section Tests
 * ============================================================================ */

TEST_F(CtIniTest, HasSection) {
    std::string content = R"([database]
host = localhost

[application]
name = MyApp
)";
    createTestFile("sections.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("sections.ini"));

    EXPECT_TRUE(profile.hasSection("database"));
    EXPECT_TRUE(profile.hasSection("application"));
    EXPECT_FALSE(profile.hasSection("nonexistent"));
}

TEST_F(CtIniTest, GetKeys) {
    std::string content = R"([database]
host = localhost
port = 5432
user = admin
)";
    createTestFile("keys.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("keys.ini"));

    std::vector<std::string> keys = profile.getKeys("database");

    EXPECT_EQ(keys.size(), 3u);
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), std::string("host")) != keys.end());
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), std::string("port")) != keys.end());
    EXPECT_TRUE(std::find(keys.begin(), keys.end(), std::string("user")) != keys.end());
}

TEST_F(CtIniTest, GetKeysFromNonexistentSection) {
    std::string content = R"([database]
host = localhost
)";
    createTestFile("empty_keys.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("empty_keys.ini"));

    std::vector<std::string> keys = profile.getKeys("nonexistent");

    EXPECT_EQ(keys.size(), 0u);
}

/* ============================================================================
 * Whitespace and Comments Tests
 * ============================================================================ */

TEST_F(CtIniTest, TrimWhitespace) {
    std::string content = R"([  database  ]
  host  =  localhost  
  port = 5432
)";
    createTestFile("whitespace.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("whitespace.ini"));

    // The section name should be trimmed
    EXPECT_TRUE(profile.hasSection("database"));
    // Values should be trimmed
    EXPECT_EQ(profile.get("database", "host"), "localhost");
    EXPECT_EQ(profile.get("database", "port"), "5432");
}

TEST_F(CtIniTest, SkipCommentLines) {
    std::string content = R"(; This is a comment
# This is also a comment
[database]
; Comment before key
host = localhost
# Another comment
port = 5432
)";
    createTestFile("comments.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("comments.ini"));

    EXPECT_TRUE(profile.hasSection("database"));
    EXPECT_EQ(profile.get("database", "host"), "localhost");
    EXPECT_EQ(profile.get("database", "port"), "5432");
}

TEST_F(CtIniTest, SkipInlineComments) {
    std::string content = R"([database]
host = localhost ; this is a comment
port = 5432 # another comment
)";
    createTestFile("inline_comments.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("inline_comments.ini"));

    EXPECT_EQ(profile.get("database", "host"), "localhost");
    EXPECT_EQ(profile.get("database", "port"), "5432");
}

TEST_F(CtIniTest, EmptyLines) {
    std::string content = R"([database]
host = localhost


port = 5432

)";
    createTestFile("empty_lines.ini", content);

    ctool::ini::Profile profile;
    bool                result = profile.load(getTempFilePath("empty_lines.ini"));

    EXPECT_TRUE(result);
    EXPECT_EQ(profile.get("database", "host"), "localhost");
    EXPECT_EQ(profile.get("database", "port"), "5432");
}

/* ============================================================================
 * Save and Persistence Tests
 * ============================================================================ */

TEST_F(CtIniTest, SaveAndReload) {
    std::string save_file = getTempFilePath("save_test.ini");

    // Create and save profile
    ctool::ini::Profile profile;
    profile.set("database", "host", "localhost");
    profile.set("database", "port", "5432");
    profile.set("application", "name", "MyApp");

    bool result = profile.save(save_file);
    EXPECT_TRUE(result);
    EXPECT_TRUE(fs::exists(save_file));

    // Load and verify
    ctool::ini::Profile loaded;
    bool                load_result = loaded.load(save_file);
    EXPECT_TRUE(load_result);

    EXPECT_EQ(loaded.get("database", "host"), "localhost");
    EXPECT_EQ(loaded.get("database", "port"), "5432");
    EXPECT_EQ(loaded.get("application", "name"), "MyApp");
}

TEST_F(CtIniTest, SaveEmptyProfile) {
    std::string save_file = getTempFilePath("empty_save.ini");

    ctool::ini::Profile profile;
    bool                result = profile.save(save_file);

    EXPECT_TRUE(result);
    EXPECT_TRUE(fs::exists(save_file));
}

TEST_F(CtIniTest, SaveToInvalidPath) {
    ctool::ini::Profile profile;
    profile.set("test", "key", "value");

    // Try to save to an invalid path (non-existent parent directory)
    std::string invalid_path = test_dir + "/nonexistent/path/file.ini";
    bool        result       = profile.save(invalid_path);

    // This should fail because the parent directory doesn't exist
    EXPECT_FALSE(result);
}

TEST_F(CtIniTest, SaveMultipleSections) {
    std::string save_file = getTempFilePath("multi_save.ini");

    ctool::ini::Profile profile;
    profile.set("section1", "key1", "value1");
    profile.set("section1", "key2", "value2");
    profile.set("section2", "key3", "value3");
    profile.set("section3", "key4", "value4");

    bool result = profile.save(save_file);
    EXPECT_TRUE(result);

    // Verify all sections are saved
    ctool::ini::Profile loaded;
    loaded.load(save_file);

    EXPECT_TRUE(loaded.hasSection("section1"));
    EXPECT_TRUE(loaded.hasSection("section2"));
    EXPECT_TRUE(loaded.hasSection("section3"));

    std::vector<std::string> keys1 = loaded.getKeys("section1");
    EXPECT_EQ(keys1.size(), 2u);
}

/* ============================================================================
 * Edge Cases and Special Values Tests
 * ============================================================================ */

TEST_F(CtIniTest, ValueWithEquals) {
    std::string content = R"([database]
connection_string = server=localhost;database=mydb;user=admin
)";
    createTestFile("equals.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("equals.ini"));

    // Note: semicolons are treated as comment delimiters, so only the first part is captured
    std::string value = profile.get("database", "connection_string");
    EXPECT_EQ(value, "server=localhost");
}

TEST_F(CtIniTest, ValueWithSpecialCharacters) {
    std::string content = R"([urls]
website = https://example.com:8080/path?query=value&other=123
email = user@example.com
path = C:\Users\Admin\Documents
)";
    createTestFile("special.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("special.ini"));

    EXPECT_EQ(profile.get("urls", "website"), "https://example.com:8080/path?query=value&other=123");
    EXPECT_EQ(profile.get("urls", "email"), "user@example.com");
    EXPECT_EQ(profile.get("urls", "path"), "C:\\Users\\Admin\\Documents");
}

TEST_F(CtIniTest, ValueWithNumbers) {
    std::string content = R"([config]
count = 12345
ratio = 3.14159
negative = -42
hex = 0xFF
)";
    createTestFile("numbers.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("numbers.ini"));

    EXPECT_EQ(profile.get("config", "count"), "12345");
    EXPECT_EQ(profile.get("config", "ratio"), "3.14159");
    EXPECT_EQ(profile.get("config", "negative"), "-42");
    EXPECT_EQ(profile.get("config", "hex"), "0xFF");
}

TEST_F(CtIniTest, EmptyValue) {
    std::string content = R"([section]
empty_key = 
normal_key = value
)";
    createTestFile("empty_value.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("empty_value.ini"));

    EXPECT_EQ(profile.get("section", "empty_key"), "");
    EXPECT_EQ(profile.get("section", "normal_key"), "value");
}

TEST_F(CtIniTest, MulitpleEqualsInValue) {
    std::string content = R"([test]
key = value=with=multiple=equals
)";
    createTestFile("multiple_equals.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("multiple_equals.ini"));

    EXPECT_EQ(profile.get("test", "key"), "value=with=multiple=equals");
}

/* ============================================================================
 * Section Name Edge Cases
 * ============================================================================ */

TEST_F(CtIniTest, SectionWithSpecialCharacters) {
    std::string content = R"([database-config_v2.0]
host = localhost
)";
    createTestFile("section_special.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("section_special.ini"));

    EXPECT_TRUE(profile.hasSection("database-config_v2.0"));
}

TEST_F(CtIniTest, CaseSensitiveKeys) {
    std::string content = R"([section]
Key = value1
key = value2
KEY = value3
)";
    createTestFile("case_sensitive.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("case_sensitive.ini"));

    EXPECT_EQ(profile.get("section", "Key"), "value1");
    EXPECT_EQ(profile.get("section", "key"), "value2");
    EXPECT_EQ(profile.get("section", "KEY"), "value3");
}

TEST_F(CtIniTest, CaseSensitiveSections) {
    std::string content = R"([Database]
key = value1

[database]
key = value2

[DATABASE]
key = value3
)";
    createTestFile("case_sections.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("case_sections.ini"));

    EXPECT_TRUE(profile.hasSection("Database"));
    EXPECT_TRUE(profile.hasSection("database"));
    EXPECT_TRUE(profile.hasSection("DATABASE"));

    EXPECT_EQ(profile.get("Database", "key"), "value1");
    EXPECT_EQ(profile.get("database", "key"), "value2");
    EXPECT_EQ(profile.get("DATABASE", "key"), "value3");
}

/* ============================================================================
 * Large File and Performance Tests
 * ============================================================================ */

TEST_F(CtIniTest, LoadLargeFile) {
    // Create a large INI file with many sections and keys
    std::string content;
    for (int i = 0; i < 100; ++i) {
        content += "[section_" + std::to_string(i) + "]\n";
        for (int j = 0; j < 50; ++j) {
            content += "key_" + std::to_string(j) + " = value_" + std::to_string(i) + "_" + std::to_string(j) + "\n";
        }
        content += "\n";
    }

    createTestFile("large.ini", content);

    ctool::ini::Profile profile;
    bool                result = profile.load(getTempFilePath("large.ini"));

    EXPECT_TRUE(result);
    EXPECT_TRUE(profile.hasSection("section_0"));
    EXPECT_TRUE(profile.hasSection("section_99"));
    EXPECT_EQ(profile.get("section_50", "key_25"), "value_50_25");
}

/* ============================================================================
 * Complex Real-World Scenarios
 * ============================================================================ */

TEST_F(CtIniTest, RealWorldApplicationConfig) {
    std::string content = R"(; Application configuration file
; Last updated: 2026-05-29

[database]
host = db.example.com
port = 5432
user = admin
password = secret123
database = myapp

[logging]
level = INFO
file = /var/log/app.log
max_size = 10485760 ; 10MB

[api]
endpoint = https://api.example.com
timeout = 30
retries = 3

[cache]
enabled = true
ttl = 3600 ; 1 hour
)";
    createTestFile("app_config.ini", content);

    ctool::ini::Profile profile;
    bool                result = profile.load(getTempFilePath("app_config.ini"));

    EXPECT_TRUE(result);
    EXPECT_EQ(profile.get("database", "host"), "db.example.com");
    EXPECT_EQ(profile.get("database", "port"), "5432");
    EXPECT_EQ(profile.get("logging", "level"), "INFO");
    EXPECT_EQ(profile.get("api", "timeout"), "30");
    EXPECT_EQ(profile.get("cache", "enabled"), "true");
}

TEST_F(CtIniTest, ModifyAndSaveExistingConfig) {
    std::string content = R"([database]
host = localhost
port = 5432

[application]
name = OldName
version = 1.0.0
)";
    createTestFile("modify.ini", content);

    ctool::ini::Profile profile;
    profile.load(getTempFilePath("modify.ini"));

    // Modify existing values
    profile.set("database", "port", "3306");
    profile.set("application", "name", "NewName");
    // Add new values
    profile.set("application", "debug", "true");
    profile.set("security", "ssl", "enabled");

    std::string modified_file = getTempFilePath("modified.ini");
    bool        save_result   = profile.save(modified_file);
    EXPECT_TRUE(save_result);

    // Reload and verify
    ctool::ini::Profile reloaded;
    reloaded.load(modified_file);

    EXPECT_EQ(reloaded.get("database", "port"), "3306");
    EXPECT_EQ(reloaded.get("application", "name"), "NewName");
    EXPECT_EQ(reloaded.get("application", "debug"), "true");
    EXPECT_EQ(reloaded.get("security", "ssl"), "enabled");
}
