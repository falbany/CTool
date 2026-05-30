/**
 * @file str_test.cpp
 * @author Florent ALBANY - FAL
 * @brief Comprehensive test suite for CBridge string module (cbridge_string) using Google Test.
 * @version 2.0
 * @date 2026-05-29
 */

#include <gtest/gtest.h>
#include <cstddef>
#include <cstring>

/* The CBridge C header has no __cplusplus guards, so wrap with extern "C" */
extern "C" {
#include "../../cbridge/string.h"
}

/* ============================================================================
 * Lifecycle & State Tests
 * ============================================================================ */

TEST(CbStrLifecycle, CreateNullString) {
    string_t* s = cbridge_string.create(NULL);
    EXPECT_NE(s, nullptr);
    EXPECT_EQ(cbridge_string.length(s), 0u);
    EXPECT_EQ(cbridge_string.c_str(s)[0], '\0');
    cbridge_string.free(s);
}

TEST(CbStrLifecycle, CreateWithString) {
    string_t* s = cbridge_string.create("Hello, World!");
    EXPECT_NE(s, nullptr);
    EXPECT_EQ(cbridge_string.length(s), 13u);
    EXPECT_STREQ(cbridge_string.c_str(s), "Hello, World!");
    cbridge_string.free(s);
}

TEST(CbStrLifecycle, CreateEmptyString) {
    string_t* s = cbridge_string.create("");
    EXPECT_NE(s, nullptr);
    EXPECT_EQ(cbridge_string.length(s), 0u);
    EXPECT_EQ(cbridge_string.c_str(s)[0], '\0');
    cbridge_string.free(s);
}

TEST(CbStrLifecycle, FreeNullString) {
    /* Should not crash */
    cbridge_string.free(NULL);
}

TEST(CbStrLifecycle, FreeString) {
    /* Should not crash or leak */
    string_t* s = cbridge_string.create("Test");
    cbridge_string.free(s);
}

TEST(CbStrLifecycle, ReserveIncrease) {
    string_t* s = cbridge_string.create("Test");
    cbridge_string.reserve(s, 100);
    EXPECT_EQ(cbridge_string.length(s), 4u);
    EXPECT_STREQ(cbridge_string.c_str(s), "Test");
    cbridge_string.free(s);
}

TEST(CbStrLifecycle, ReserveDecrease) {
    string_t* s                 = cbridge_string.create("Test");
    size_t    original_capacity = s->capacity;
    cbridge_string.reserve(s, 1); /* Try to decrease capacity */
    EXPECT_EQ(s->capacity, original_capacity) << "reserve should not decrease capacity";
    cbridge_string.free(s);
}

TEST(CbStrLifecycle, ClearString) {
    string_t* s = cbridge_string.create("Hello");
    cbridge_string.clear(s);
    EXPECT_TRUE(cbridge_string.empty(s));
    EXPECT_EQ(cbridge_string.length(s), 0u);
    EXPECT_STREQ("", cbridge_string.c_str(s));
    EXPECT_EQ(cbridge_string.c_str(s)[0], '\0');
    cbridge_string.free(s);
}

TEST(CbStrLifecycle, EmptyTrue) {
    string_t* s = cbridge_string.create("");
    EXPECT_TRUE(cbridge_string.empty(s));
    EXPECT_EQ(cbridge_string.length(s), 0u);
    cbridge_string.free(s);
}

TEST(CbStrLifecycle, EmptyFalse) {
    string_t* s = cbridge_string.create("Not empty");
    EXPECT_FALSE(cbridge_string.empty(s));
    EXPECT_GT(cbridge_string.length(s), 0u);
    cbridge_string.free(s);
}

TEST(CbStrLifecycle, LengthBasic) {
    string_t* s = cbridge_string.create("Hello, World!");
    EXPECT_EQ(cbridge_string.length(s), 13u);
    cbridge_string.free(s);
}

TEST(CbStrLifecycle, CStrBasic) {
    string_t* s = cbridge_string.create("Test String");
    EXPECT_STREQ("Test String", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

/* ============================================================================
 * Modifier Tests - append
 * ============================================================================ */

TEST(CbStrAppend, AppendSingle) {
    string_t* s = cbridge_string.create("Hello");
    cbridge_string.append(s, ", World");
    EXPECT_EQ(cbridge_string.length(s), 12u);
    EXPECT_STREQ("Hello, World", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrAppend, AppendMultiple) {
    string_t* s = cbridge_string.create("Start");
    cbridge_string.append(s, " ");
    cbridge_string.append(s, "middle");
    cbridge_string.append(s, " end");
    EXPECT_EQ(cbridge_string.length(s), 16u);
    EXPECT_STREQ("Start middle end", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrAppend, AppendNullSuffix) {
    string_t* s = cbridge_string.create("Test");
    cbridge_string.append(s, NULL); /* Should not crash or modify */
    EXPECT_EQ(cbridge_string.length(s), 4u);
    EXPECT_STREQ("Test", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrAppend, AppendToEmpty) {
    string_t* s = cbridge_string.create("");
    cbridge_string.append(s, "Hello");
    EXPECT_EQ(cbridge_string.length(s), 5u);
    EXPECT_STREQ("Hello", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

/* ============================================================================
 * Modifier Tests - insert
 * ============================================================================ */

TEST(CbStrInsert, InsertAtBeginning) {
    string_t* s = cbridge_string.create("World");
    cbridge_string.insert(s, 0, "Hello ");
    EXPECT_EQ(cbridge_string.length(s), 11u);
    EXPECT_STREQ("Hello World", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrInsert, InsertAtEnd) {
    string_t* s = cbridge_string.create("Hello");
    cbridge_string.insert(s, 5, " World");
    EXPECT_EQ(cbridge_string.length(s), 11u);
    EXPECT_STREQ("Hello World", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrInsert, InsertInMiddle) {
    string_t* s = cbridge_string.create("Hlo");
    cbridge_string.insert(s, 2, "el");
    EXPECT_EQ(cbridge_string.length(s), 5u);
    EXPECT_STREQ("Hlelo", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrInsert, InsertClampToEnd) {
    string_t* s = cbridge_string.create("Test");
    cbridge_string.insert(s, 100, "XX"); /* Position > length, should clamp */
    EXPECT_EQ(cbridge_string.length(s), 6u);
    EXPECT_STREQ("TestXX", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrInsert, InsertEmptyString) {
    string_t* s = cbridge_string.create("Test");
    cbridge_string.insert(s, 0, ""); /* Empty insert, should do nothing */
    EXPECT_EQ(cbridge_string.length(s), 4u);
    EXPECT_STREQ("Test", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrInsert, InsertNullStr) {
    string_t* s = cbridge_string.create("Test");
    cbridge_string.insert(s, 0, NULL); /* Should not crash */
    EXPECT_EQ(cbridge_string.length(s), 4u);
    cbridge_string.free(s);
}

/* ============================================================================
 * Modifier Tests - erase
 * ============================================================================ */

TEST(CbStrErase, EraseFromStart) {
    string_t* s = cbridge_string.create("Hello World");
    cbridge_string.erase(s, 0, 6); /* Remove "Hello " */
    EXPECT_EQ(cbridge_string.length(s), 5u);
    EXPECT_STREQ("World", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrErase, EraseMiddle) {
    string_t* s = cbridge_string.create("1234567890");
    cbridge_string.erase(s, 3, 4); /* Remove "4567" */
    EXPECT_EQ(cbridge_string.length(s), 6u);
    EXPECT_STREQ("123890", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrErase, EraseToEnd) {
    string_t* s = cbridge_string.create("Test String");
    cbridge_string.erase(s, 5, 100); /* Erase more than remaining */
    EXPECT_EQ(cbridge_string.length(s), 5u);
    EXPECT_STREQ("Test ", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrErase, EraseInvalidPos) {
    string_t* s = cbridge_string.create("Test");
    cbridge_string.erase(s, 100, 3); /* pos >= length, should do nothing */
    EXPECT_EQ(cbridge_string.length(s), 4u);
    cbridge_string.free(s);
}

TEST(CbStrErase, EraseNullLen) {
    string_t* s = cbridge_string.create("Test");
    cbridge_string.erase(s, 0, 0); /* len == 0, should do nothing */
    EXPECT_EQ(cbridge_string.length(s), 4u);
    cbridge_string.free(s);
}

/* ============================================================================
 * Modifier Tests - trim
 * ============================================================================ */

TEST(CbStrTrim, TrimLeadingSpaces) {
    string_t* s = cbridge_string.create("   Hello");
    cbridge_string.trim(s);
    EXPECT_STREQ("Hello", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrTrim, TrimTrailingSpaces) {
    string_t* s = cbridge_string.create("Hello   ");
    cbridge_string.trim(s);
    EXPECT_STREQ("Hello", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrTrim, TrimBothSides) {
    string_t* s = cbridge_string.create("  Hello World  ");
    cbridge_string.trim(s);
    EXPECT_STREQ("Hello World", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrTrim, TrimAllSpaces) {
    string_t* s = cbridge_string.create("     ");
    cbridge_string.trim(s);
    EXPECT_TRUE(cbridge_string.empty(s));
    EXPECT_EQ(cbridge_string.length(s), 0u);
    cbridge_string.free(s);
}

TEST(CbStrTrim, TrimNoSpaces) {
    string_t* s = cbridge_string.create("No spaces here");
    cbridge_string.trim(s);
    EXPECT_STREQ("No spaces here", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrTrim, TrimTabsAndNewlines) {
    string_t* s = cbridge_string.create("\t\nHello\t\n");
    cbridge_string.trim(s);
    EXPECT_STREQ("Hello", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

/* ============================================================================
 * Modifier Tests - replace_all
 * ============================================================================ */

TEST(CbStrReplaceAll, ReplaceAllBasic) {
    string_t* s = cbridge_string.create("Hello World");
    cbridge_string.replace_all(s, "World", "Universe");
    EXPECT_STREQ("Hello Universe", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrReplaceAll, ReplaceAllMultiple) {
    string_t* s = cbridge_string.create("AAA BBB AAA");
    cbridge_string.replace_all(s, "AAA", "XXX");
    EXPECT_STREQ("XXX BBB XXX", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrReplaceAll, ReplaceAllNoMatch) {
    string_t* s = cbridge_string.create("Hello World");
    cbridge_string.replace_all(s, "Universe", "Galaxy"); /* No match */
    EXPECT_STREQ("Hello World", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrReplaceAll, ReplaceAllEmptyReplace) {
    string_t* s = cbridge_string.create("Hello");
    cbridge_string.replace_all(s, "l", ""); /* Replace with empty */
    EXPECT_STREQ("Heo", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrReplaceAll, ReplaceAllOverlapping) {
    string_t* s = cbridge_string.create("aaaa");
    cbridge_string.replace_all(s, "aa", "b");
    EXPECT_STREQ("bb", cbridge_string.c_str(s)) << "Non-overlapping replacements";
    cbridge_string.free(s);
}

/* ============================================================================
 * Operation Tests - compare
 * ============================================================================ */

TEST(CbStrCompare, CompareEqual) {
    string_t* s = cbridge_string.create("Test");
    EXPECT_EQ(cbridge_string.compare(s, "Test"), 0);
    cbridge_string.free(s);
}

TEST(CbStrCompare, CompareGreater) {
    string_t* s = cbridge_string.create("Zebra");
    EXPECT_GT(cbridge_string.compare(s, "Apple"), 0);
    cbridge_string.free(s);
}

TEST(CbStrCompare, CompareLess) {
    string_t* s = cbridge_string.create("Apple");
    EXPECT_LT(cbridge_string.compare(s, "Zebra"), 0);
    cbridge_string.free(s);
}

TEST(CbStrCompare, CompareWithNull) {
    string_t* s = cbridge_string.create("Test");
    EXPECT_GT(cbridge_string.compare(s, NULL), 0);
    cbridge_string.free(s);
}

TEST(CbStrCompare, CompareCaseSensitivity) {
    string_t* s = cbridge_string.create("Test");
    EXPECT_LT(cbridge_string.compare(s, "test"), 0) << "Comparison should be case-sensitive (uppercase < lowercase)";
    cbridge_string.free(s);
}

/* ============================================================================
 * Operation Tests - starts_with
 * ============================================================================ */

TEST(CbStrStartsWith, StartsWithTrue) {
    string_t* s = cbridge_string.create("Hello World");
    EXPECT_TRUE(cbridge_string.starts_with(s, "Hello"));
    cbridge_string.free(s);
}

TEST(CbStrStartsWith, StartsWithFalse) {
    string_t* s = cbridge_string.create("Hello World");
    EXPECT_FALSE(cbridge_string.starts_with(s, "World"));
    cbridge_string.free(s);
}

TEST(CbStrStartsWith, StartsWithExactMatch) {
    string_t* s = cbridge_string.create("Test");
    EXPECT_TRUE(cbridge_string.starts_with(s, "Test"));
    cbridge_string.free(s);
}

TEST(CbStrStartsWith, StartsWithEmptyPrefix) {
    string_t* s = cbridge_string.create("Test");
    EXPECT_TRUE(cbridge_string.starts_with(s, ""));
    cbridge_string.free(s);
}

TEST(CbStrStartsWith, StartsWithLongerThanString) {
    string_t* s = cbridge_string.create("Hi");
    EXPECT_FALSE(cbridge_string.starts_with(s, "Hello"));
    cbridge_string.free(s);
}

/* ============================================================================
 * Operation Tests - ends_with
 * ============================================================================ */

TEST(CbStrEndsWith, EndsWithTrue) {
    string_t* s = cbridge_string.create("Hello World");
    EXPECT_TRUE(cbridge_string.ends_with(s, "World"));
    cbridge_string.free(s);
}

TEST(CbStrEndsWith, EndsWithFalse) {
    string_t* s = cbridge_string.create("Hello World");
    EXPECT_FALSE(cbridge_string.ends_with(s, "Hello"));
    cbridge_string.free(s);
}

TEST(CbStrEndsWith, EndsWithExactMatch) {
    string_t* s = cbridge_string.create("Test");
    EXPECT_TRUE(cbridge_string.ends_with(s, "Test"));
    cbridge_string.free(s);
}

TEST(CbStrEndsWith, EndsWithEmptySuffix) {
    string_t* s = cbridge_string.create("Test");
    EXPECT_TRUE(cbridge_string.ends_with(s, ""));
    cbridge_string.free(s);
}

TEST(CbStrEndsWith, EndsWithLongerThanString) {
    string_t* s = cbridge_string.create("Hi");
    EXPECT_FALSE(cbridge_string.ends_with(s, "Hello"));
    cbridge_string.free(s);
}

/* ============================================================================
 * Operation Tests - find
 * ============================================================================ */

TEST(CbStrFind, FindFirstOccurrence) {
    string_t* s   = cbridge_string.create("Hello World");
    size_t    pos = cbridge_string.find(s, "World", 0);
    EXPECT_EQ(pos, 6u);
    cbridge_string.free(s);
}

TEST(CbStrFind, FindNotFound) {
    string_t* s   = cbridge_string.create("Hello World");
    size_t    pos = cbridge_string.find(s, "Universe", 0);
    EXPECT_EQ(pos, CBRIDGE_STR_NPOS);
    cbridge_string.free(s);
}

TEST(CbStrFind, FindStartPos) {
    string_t* s   = cbridge_string.create("aaa aaa aaa");
    size_t    pos = cbridge_string.find(s, "a", 3); /* Start searching from position 3 (space), first 'a' after at 4 */
    EXPECT_EQ(pos, 4u);
    cbridge_string.free(s);
}

TEST(CbStrFind, FindEmptyStringFound) {
    string_t* s   = cbridge_string.create("Test");
    size_t    pos = cbridge_string.find(s, "", 0); /* Empty string */
    EXPECT_EQ(pos, 0u);
    cbridge_string.free(s);
}

TEST(CbStrFind, FindFromPositionGreaterThanLength) {
    string_t* s   = cbridge_string.create("Test");
    size_t    pos = cbridge_string.find(s, "e", 100); /* Start position > length */
    EXPECT_EQ(pos, CBRIDGE_STR_NPOS);
    cbridge_string.free(s);
}

/* ============================================================================
 * Operation Tests - substr
 * ============================================================================ */

TEST(CbStrSubstr, SubstrBasic) {
    string_t* s   = cbridge_string.create("Hello World");
    string_t* sub = cbridge_string.substr(s, 6, 5);
    EXPECT_STREQ("World", cbridge_string.c_str(sub));
    EXPECT_EQ(cbridge_string.length(sub), 5u);
    cbridge_string.free(sub);
    cbridge_string.free(s);
}

TEST(CbStrSubstr, SubstrFromBeginning) {
    string_t* s   = cbridge_string.create("Hello World");
    string_t* sub = cbridge_string.substr(s, 0, 5);
    EXPECT_STREQ("Hello", cbridge_string.c_str(sub));
    cbridge_string.free(sub);
    cbridge_string.free(s);
}

TEST(CbStrSubstr, SubstrToEnd) {
    string_t* s   = cbridge_string.create("Hello World");
    string_t* sub = cbridge_string.substr(s, 6, 100); /* More than remaining */
    EXPECT_STREQ("World", cbridge_string.c_str(sub));
    cbridge_string.free(sub);
    cbridge_string.free(s);
}

TEST(CbStrSubstr, SubstrInvalidPosition) {
    string_t* s   = cbridge_string.create("Test");
    string_t* sub = cbridge_string.substr(s, 100, 5); /* Invalid position */
    EXPECT_TRUE(cbridge_string.empty(sub));
    cbridge_string.free(sub);
    cbridge_string.free(s);
}

TEST(CbStrSubstr, SubstrEmptyString) {
    string_t* s   = cbridge_string.create("");
    string_t* sub = cbridge_string.substr(s, 0, 0);
    EXPECT_TRUE(cbridge_string.empty(sub));
    cbridge_string.free(sub);
    cbridge_string.free(s);
}

/* ============================================================================
 * Operation Tests - catch_in_range
 * ============================================================================ */

TEST(CbStrCatchInRange, CatchInRangeBasic) {
    string_t* s      = cbridge_string.create("Request: GET /api/users");
    string_t* result = cbridge_string.catch_in_range(s, ": ", " ");
    EXPECT_STREQ("GET", cbridge_string.c_str(result));
    cbridge_string.free(result);
    cbridge_string.free(s);
}

TEST(CbStrCatchInRange, CatchInRangeNoStart) {
    string_t* s      = cbridge_string.create("Hello World");
    string_t* result = cbridge_string.catch_in_range(s, "XX", "YY");
    EXPECT_TRUE(cbridge_string.empty(result));
    cbridge_string.free(result);
    cbridge_string.free(s);
}

TEST(CbStrCatchInRange, CatchInRangeNoEnd) {
    string_t* s      = cbridge_string.create("Hello World");
    string_t* result = cbridge_string.catch_in_range(s, "Hello", "XXX");
    EXPECT_TRUE(cbridge_string.empty(result));
    cbridge_string.free(result);
    cbridge_string.free(s);
}

TEST(CbStrCatchInRange, CatchInRangeEmptyContent) {
    string_t* s      = cbridge_string.create("Start End");
    string_t* result = cbridge_string.catch_in_range(s, "Start ", " End");
    EXPECT_TRUE(cbridge_string.empty(result));
    cbridge_string.free(result);
    cbridge_string.free(s);
}

/* ============================================================================
 * Split Tests
 * ============================================================================ */

TEST(CbStrSplit, SplitBasic) {
    string_t*      s     = cbridge_string.create("one,two,three");
    string_parts_t parts = cbridge_string.split(s, ",");

    EXPECT_EQ(parts.count, 3u);
    EXPECT_NE(parts.parts, nullptr);
    EXPECT_STREQ("one", cbridge_string.c_str(parts.parts[0]));
    EXPECT_STREQ("two", cbridge_string.c_str(parts.parts[1]));
    EXPECT_STREQ("three", cbridge_string.c_str(parts.parts[2]));

    cbridge_str_parts_free(&parts);
    cbridge_string.free(s);
}

TEST(CbStrSplit, SplitSingleToken) {
    string_t*      s     = cbridge_string.create("no_delimiters");
    string_parts_t parts = cbridge_string.split(s, ",");

    EXPECT_EQ(parts.count, 1u);
    EXPECT_STREQ("no_delimiters", cbridge_string.c_str(parts.parts[0]));

    cbridge_str_parts_free(&parts);
    cbridge_string.free(s);
}

TEST(CbStrSplit, SplitEmptyString) {
    string_t*      s     = cbridge_string.create("");
    string_parts_t parts = cbridge_string.split(s, ",");

    EXPECT_EQ(parts.count, 0u);
    EXPECT_EQ(parts.parts, nullptr);

    cbridge_str_parts_free(&parts);
    cbridge_string.free(s);
}

TEST(CbStrSplit, SplitNullDelimiter) {
    string_t*      s     = cbridge_string.create("Test String");
    string_parts_t parts = cbridge_string.split(s, NULL);

    EXPECT_EQ(parts.count, 1u);
    EXPECT_STREQ("Test String", cbridge_string.c_str(parts.parts[0]));

    cbridge_str_parts_free(&parts);
    cbridge_string.free(s);
}

TEST(CbStrSplit, SplitEmptyDelimiter) {
    string_t*      s     = cbridge_string.create("Test");
    string_parts_t parts = cbridge_string.split(s, "");

    EXPECT_EQ(parts.count, 1u);
    EXPECT_STREQ("Test", cbridge_string.c_str(parts.parts[0]));

    cbridge_str_parts_free(&parts);
    cbridge_string.free(s);
}

TEST(CbStrSplit, SplitMultipleDelimiters) {
    string_t*      s     = cbridge_string.create("a,,b,c");
    string_parts_t parts = cbridge_string.split(s, ",");

    EXPECT_EQ(parts.count, 4u);
    EXPECT_STREQ("a", cbridge_string.c_str(parts.parts[0]));
    EXPECT_STREQ("", cbridge_string.c_str(parts.parts[1]));
    EXPECT_STREQ("b", cbridge_string.c_str(parts.parts[2]));
    EXPECT_STREQ("c", cbridge_string.c_str(parts.parts[3]));

    cbridge_str_parts_free(&parts);
    cbridge_string.free(s);
}

TEST(CbStrSplit, SplitTrailingDelimiter) {
    string_t*      s     = cbridge_string.create("a,b,c,");
    string_parts_t parts = cbridge_string.split(s, ",");

    EXPECT_EQ(parts.count, 4u);
    EXPECT_STREQ("a", cbridge_string.c_str(parts.parts[0]));
    EXPECT_STREQ("b", cbridge_string.c_str(parts.parts[1]));
    EXPECT_STREQ("c", cbridge_string.c_str(parts.parts[2]));
    EXPECT_STREQ("", cbridge_string.c_str(parts.parts[3]));

    cbridge_str_parts_free(&parts);
    cbridge_string.free(s);
}

TEST(CbStrSplit, PartsFreeClearsStruct) {
    string_t*      s     = cbridge_string.create("a,b");
    string_parts_t parts = cbridge_string.split(s, ",");
    cbridge_str_parts_free(&parts);

    EXPECT_EQ(parts.count, 0u);
    EXPECT_EQ(parts.parts, nullptr);
    cbridge_string.free(s);
}

/* ============================================================================
 * Edge Cases & Stress Tests
 * ============================================================================ */

TEST(CbStrEdgeCases, LargeStringAllocation) {
    char large_buf[200];
    for (int i = 0; i < 199; i++) large_buf[i] = 'A' + (i % 26);
    large_buf[199] = '\0';

    string_t* s = cbridge_string.create(large_buf);
    EXPECT_NE(s, nullptr);
    EXPECT_EQ(cbridge_string.length(s), 199u);
    EXPECT_STREQ(large_buf, cbridge_string.c_str(s));

    cbridge_string.free(s);
}

TEST(CbStrEdgeCases, RepeatedAppend) {
    string_t* s = cbridge_string.create("A");
    for (int i = 0; i < 50; i++) {
        cbridge_string.append(s, "B");
    }
    EXPECT_EQ(cbridge_string.length(s), 51u);
    EXPECT_EQ(cbridge_string.c_str(s)[0], 'A');
    for (size_t i = 1; i < cbridge_string.length(s); i++) {
        EXPECT_EQ(cbridge_string.c_str(s)[i], 'B');
    }

    cbridge_string.free(s);
}

TEST(CbStrEdgeCases, NullStringOperations) {
    /* These operations should handle NULL gracefully */
    EXPECT_EQ(cbridge_string.length(NULL), 0u);
    EXPECT_TRUE(cbridge_string.empty(NULL));
    EXPECT_STREQ("", cbridge_string.c_str(NULL));
}

TEST(CbStrEdgeCases, EscapeChars) {
    string_t* s = cbridge_string.create("Tab:\tNewline:\nBackslash:\\");
    EXPECT_EQ(cbridge_string.length(s), 25u);
    cbridge_string.free(s);
}

TEST(CbStrEdgeCases, MultidimensionalSpaceTrim) {
    string_t* s = cbridge_string.create("  \t  \n  Trim Me  \n\t  ");
    cbridge_string.trim(s);
    EXPECT_STREQ("Trim Me", cbridge_string.c_str(s));
    cbridge_string.free(s);
}

TEST(CbStrEdgeCases, UnicodeFriendlyAscii) {
    string_t* s = cbridge_string.create("ASCII only: 12345!");
    cbridge_string.append(s, " More text");
    EXPECT_STREQ("ASCII only: 12345! More text", cbridge_string.c_str(s));
    cbridge_string.free(s);
}
