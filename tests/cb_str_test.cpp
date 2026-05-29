/**
 * @file cb_str_test.cpp
 * @author Florent ALBANY - FAL
 * @brief Comprehensive test suite for CBridge string module (cb_str) using Google Test.
 * @version 2.0
 * @date 2026-05-29
 */

#include <gtest/gtest.h>
#include <cstddef>
#include <cstring>

/* The CBridge C header has no __cplusplus guards, so wrap with extern "C" */
extern "C" {
#include "cb/cb_str.h"
}

/* ============================================================================
 * Lifecycle & State Tests
 * ============================================================================ */

TEST(CbStrLifecycle, CreateNullString) {
    cb_string_t* s = cb_str.create(NULL);
    EXPECT_NE(s, nullptr);
    EXPECT_EQ(cb_str.length(s), 0u);
    EXPECT_EQ(cb_str.c_str(s)[0], '\0');
    cb_str.free(s);
}

TEST(CbStrLifecycle, CreateWithString) {
    cb_string_t* s = cb_str.create("Hello, World!");
    EXPECT_NE(s, nullptr);
    EXPECT_EQ(cb_str.length(s), 13u);
    EXPECT_STREQ(cb_str.c_str(s), "Hello, World!");
    cb_str.free(s);
}

TEST(CbStrLifecycle, CreateEmptyString) {
    cb_string_t* s = cb_str.create("");
    EXPECT_NE(s, nullptr);
    EXPECT_EQ(cb_str.length(s), 0u);
    EXPECT_EQ(cb_str.c_str(s)[0], '\0');
    cb_str.free(s);
}

TEST(CbStrLifecycle, FreeNullString) {
    /* Should not crash */
    cb_str.free(NULL);
}

TEST(CbStrLifecycle, FreeString) {
    /* Should not crash or leak */
    cb_string_t* s = cb_str.create("Test");
    cb_str.free(s);
}

TEST(CbStrLifecycle, ReserveIncrease) {
    cb_string_t* s = cb_str.create("Test");
    cb_str.reserve(s, 100);
    EXPECT_EQ(cb_str.length(s), 4u);
    EXPECT_STREQ(cb_str.c_str(s), "Test");
    cb_str.free(s);
}

TEST(CbStrLifecycle, ReserveDecrease) {
    cb_string_t* s = cb_str.create("Test");
    size_t original_capacity = s->capacity;
    cb_str.reserve(s, 1); /* Try to decrease capacity */
    EXPECT_EQ(s->capacity, original_capacity) << "reserve should not decrease capacity";
    cb_str.free(s);
}

TEST(CbStrLifecycle, ClearString) {
    cb_string_t* s = cb_str.create("Hello");
    cb_str.clear(s);
    EXPECT_TRUE(cb_str.empty(s));
    EXPECT_EQ(cb_str.length(s), 0u);
    EXPECT_STREQ("", cb_str.c_str(s));
    EXPECT_EQ(cb_str.c_str(s)[0], '\0');
    cb_str.free(s);
}

TEST(CbStrLifecycle, EmptyTrue) {
    cb_string_t* s = cb_str.create("");
    EXPECT_TRUE(cb_str.empty(s));
    EXPECT_EQ(cb_str.length(s), 0u);
    cb_str.free(s);
}

TEST(CbStrLifecycle, EmptyFalse) {
    cb_string_t* s = cb_str.create("Not empty");
    EXPECT_FALSE(cb_str.empty(s));
    EXPECT_GT(cb_str.length(s), 0u);
    cb_str.free(s);
}

TEST(CbStrLifecycle, LengthBasic) {
    cb_string_t* s = cb_str.create("Hello, World!");
    EXPECT_EQ(cb_str.length(s), 13u);
    cb_str.free(s);
}

TEST(CbStrLifecycle, CStrBasic) {
    cb_string_t* s = cb_str.create("Test String");
    EXPECT_STREQ("Test String", cb_str.c_str(s));
    cb_str.free(s);
}

/* ============================================================================
 * Modifier Tests - append
 * ============================================================================ */

TEST(CbStrAppend, AppendSingle) {
    cb_string_t* s = cb_str.create("Hello");
    cb_str.append(s, ", World");
    EXPECT_EQ(cb_str.length(s), 12u);
    EXPECT_STREQ("Hello, World", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrAppend, AppendMultiple) {
    cb_string_t* s = cb_str.create("Start");
    cb_str.append(s, " ");
    cb_str.append(s, "middle");
    cb_str.append(s, " end");
    EXPECT_EQ(cb_str.length(s), 16u);
    EXPECT_STREQ("Start middle end", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrAppend, AppendNullSuffix) {
    cb_string_t* s = cb_str.create("Test");
    cb_str.append(s, NULL); /* Should not crash or modify */
    EXPECT_EQ(cb_str.length(s), 4u);
    EXPECT_STREQ("Test", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrAppend, AppendToEmpty) {
    cb_string_t* s = cb_str.create("");
    cb_str.append(s, "Hello");
    EXPECT_EQ(cb_str.length(s), 5u);
    EXPECT_STREQ("Hello", cb_str.c_str(s));
    cb_str.free(s);
}

/* ============================================================================
 * Modifier Tests - insert
 * ============================================================================ */

TEST(CbStrInsert, InsertAtBeginning) {
    cb_string_t* s = cb_str.create("World");
    cb_str.insert(s, 0, "Hello ");
    EXPECT_EQ(cb_str.length(s), 11u);
    EXPECT_STREQ("Hello World", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrInsert, InsertAtEnd) {
    cb_string_t* s = cb_str.create("Hello");
    cb_str.insert(s, 5, " World");
    EXPECT_EQ(cb_str.length(s), 11u);
    EXPECT_STREQ("Hello World", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrInsert, InsertInMiddle) {
    cb_string_t* s = cb_str.create("Hlo");
    cb_str.insert(s, 2, "el");
    EXPECT_EQ(cb_str.length(s), 5u);
    EXPECT_STREQ("Hlelo", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrInsert, InsertClampToEnd) {
    cb_string_t* s = cb_str.create("Test");
    cb_str.insert(s, 100, "XX"); /* Position > length, should clamp */
    EXPECT_EQ(cb_str.length(s), 6u);
    EXPECT_STREQ("TestXX", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrInsert, InsertEmptyString) {
    cb_string_t* s = cb_str.create("Test");
    cb_str.insert(s, 0, ""); /* Empty insert, should do nothing */
    EXPECT_EQ(cb_str.length(s), 4u);
    EXPECT_STREQ("Test", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrInsert, InsertNullStr) {
    cb_string_t* s = cb_str.create("Test");
    cb_str.insert(s, 0, NULL); /* Should not crash */
    EXPECT_EQ(cb_str.length(s), 4u);
    cb_str.free(s);
}

/* ============================================================================
 * Modifier Tests - erase
 * ============================================================================ */

TEST(CbStrErase, EraseFromStart) {
    cb_string_t* s = cb_str.create("Hello World");
    cb_str.erase(s, 0, 6); /* Remove "Hello " */
    EXPECT_EQ(cb_str.length(s), 5u);
    EXPECT_STREQ("World", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrErase, EraseMiddle) {
    cb_string_t* s = cb_str.create("1234567890");
    cb_str.erase(s, 3, 4); /* Remove "4567" */
    EXPECT_EQ(cb_str.length(s), 6u);
    EXPECT_STREQ("123890", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrErase, EraseToEnd) {
    cb_string_t* s = cb_str.create("Test String");
    cb_str.erase(s, 5, 100); /* Erase more than remaining */
    EXPECT_EQ(cb_str.length(s), 5u);
    EXPECT_STREQ("Test ", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrErase, EraseInvalidPos) {
    cb_string_t* s = cb_str.create("Test");
    cb_str.erase(s, 100, 3); /* pos >= length, should do nothing */
    EXPECT_EQ(cb_str.length(s), 4u);
    cb_str.free(s);
}

TEST(CbStrErase, EraseNullLen) {
    cb_string_t* s = cb_str.create("Test");
    cb_str.erase(s, 0, 0); /* len == 0, should do nothing */
    EXPECT_EQ(cb_str.length(s), 4u);
    cb_str.free(s);
}

/* ============================================================================
 * Modifier Tests - trim
 * ============================================================================ */

TEST(CbStrTrim, TrimLeadingSpaces) {
    cb_string_t* s = cb_str.create("   Hello");
    cb_str.trim(s);
    EXPECT_STREQ("Hello", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrTrim, TrimTrailingSpaces) {
    cb_string_t* s = cb_str.create("Hello   ");
    cb_str.trim(s);
    EXPECT_STREQ("Hello", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrTrim, TrimBothSides) {
    cb_string_t* s = cb_str.create("  Hello World  ");
    cb_str.trim(s);
    EXPECT_STREQ("Hello World", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrTrim, TrimAllSpaces) {
    cb_string_t* s = cb_str.create("     ");
    cb_str.trim(s);
    EXPECT_TRUE(cb_str.empty(s));
    EXPECT_EQ(cb_str.length(s), 0u);
    cb_str.free(s);
}

TEST(CbStrTrim, TrimNoSpaces) {
    cb_string_t* s = cb_str.create("No spaces here");
    cb_str.trim(s);
    EXPECT_STREQ("No spaces here", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrTrim, TrimTabsAndNewlines) {
    cb_string_t* s = cb_str.create("\t\nHello\t\n");
    cb_str.trim(s);
    EXPECT_STREQ("Hello", cb_str.c_str(s));
    cb_str.free(s);
}

/* ============================================================================
 * Modifier Tests - replace_all
 * ============================================================================ */

TEST(CbStrReplaceAll, ReplaceAllBasic) {
    cb_string_t* s = cb_str.create("Hello World");
    cb_str.replace_all(s, "World", "Universe");
    EXPECT_STREQ("Hello Universe", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrReplaceAll, ReplaceAllMultiple) {
    cb_string_t* s = cb_str.create("AAA BBB AAA");
    cb_str.replace_all(s, "AAA", "XXX");
    EXPECT_STREQ("XXX BBB XXX", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrReplaceAll, ReplaceAllNoMatch) {
    cb_string_t* s = cb_str.create("Hello World");
    cb_str.replace_all(s, "Universe", "Galaxy"); /* No match */
    EXPECT_STREQ("Hello World", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrReplaceAll, ReplaceAllEmptyReplace) {
    cb_string_t* s = cb_str.create("Hello");
    cb_str.replace_all(s, "l", ""); /* Replace with empty */
    EXPECT_STREQ("Heo", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrReplaceAll, ReplaceAllOverlapping) {
    cb_string_t* s = cb_str.create("aaaa");
    cb_str.replace_all(s, "aa", "b");
    EXPECT_STREQ("bb", cb_str.c_str(s)) << "Non-overlapping replacements";
    cb_str.free(s);
}

/* ============================================================================
 * Operation Tests - compare
 * ============================================================================ */

TEST(CbStrCompare, CompareEqual) {
    cb_string_t* s = cb_str.create("Test");
    EXPECT_EQ(cb_str.compare(s, "Test"), 0);
    cb_str.free(s);
}

TEST(CbStrCompare, CompareGreater) {
    cb_string_t* s = cb_str.create("Zebra");
    EXPECT_GT(cb_str.compare(s, "Apple"), 0);
    cb_str.free(s);
}

TEST(CbStrCompare, CompareLess) {
    cb_string_t* s = cb_str.create("Apple");
    EXPECT_LT(cb_str.compare(s, "Zebra"), 0);
    cb_str.free(s);
}

TEST(CbStrCompare, CompareWithNull) {
    cb_string_t* s = cb_str.create("Test");
    EXPECT_GT(cb_str.compare(s, NULL), 0);
    cb_str.free(s);
}

TEST(CbStrCompare, CompareCaseSensitivity) {
    cb_string_t* s = cb_str.create("Test");
    EXPECT_LT(cb_str.compare(s, "test"), 0)
        << "Comparison should be case-sensitive (uppercase < lowercase)";
    cb_str.free(s);
}

/* ============================================================================
 * Operation Tests - starts_with
 * ============================================================================ */

TEST(CbStrStartsWith, StartsWithTrue) {
    cb_string_t* s = cb_str.create("Hello World");
    EXPECT_TRUE(cb_str.starts_with(s, "Hello"));
    cb_str.free(s);
}

TEST(CbStrStartsWith, StartsWithFalse) {
    cb_string_t* s = cb_str.create("Hello World");
    EXPECT_FALSE(cb_str.starts_with(s, "World"));
    cb_str.free(s);
}

TEST(CbStrStartsWith, StartsWithExactMatch) {
    cb_string_t* s = cb_str.create("Test");
    EXPECT_TRUE(cb_str.starts_with(s, "Test"));
    cb_str.free(s);
}

TEST(CbStrStartsWith, StartsWithEmptyPrefix) {
    cb_string_t* s = cb_str.create("Test");
    EXPECT_TRUE(cb_str.starts_with(s, ""));
    cb_str.free(s);
}

TEST(CbStrStartsWith, StartsWithLongerThanString) {
    cb_string_t* s = cb_str.create("Hi");
    EXPECT_FALSE(cb_str.starts_with(s, "Hello"));
    cb_str.free(s);
}

/* ============================================================================
 * Operation Tests - ends_with
 * ============================================================================ */

TEST(CbStrEndsWith, EndsWithTrue) {
    cb_string_t* s = cb_str.create("Hello World");
    EXPECT_TRUE(cb_str.ends_with(s, "World"));
    cb_str.free(s);
}

TEST(CbStrEndsWith, EndsWithFalse) {
    cb_string_t* s = cb_str.create("Hello World");
    EXPECT_FALSE(cb_str.ends_with(s, "Hello"));
    cb_str.free(s);
}

TEST(CbStrEndsWith, EndsWithExactMatch) {
    cb_string_t* s = cb_str.create("Test");
    EXPECT_TRUE(cb_str.ends_with(s, "Test"));
    cb_str.free(s);
}

TEST(CbStrEndsWith, EndsWithEmptySuffix) {
    cb_string_t* s = cb_str.create("Test");
    EXPECT_TRUE(cb_str.ends_with(s, ""));
    cb_str.free(s);
}

TEST(CbStrEndsWith, EndsWithLongerThanString) {
    cb_string_t* s = cb_str.create("Hi");
    EXPECT_FALSE(cb_str.ends_with(s, "Hello"));
    cb_str.free(s);
}

/* ============================================================================
 * Operation Tests - find
 * ============================================================================ */

TEST(CbStrFind, FindFirstOccurrence) {
    cb_string_t* s = cb_str.create("Hello World");
    size_t pos = cb_str.find(s, "World", 0);
    EXPECT_EQ(pos, 6u);
    cb_str.free(s);
}

TEST(CbStrFind, FindNotFound) {
    cb_string_t* s = cb_str.create("Hello World");
    size_t pos = cb_str.find(s, "Universe", 0);
    EXPECT_EQ(pos, CB_STR_NPOS);
    cb_str.free(s);
}

TEST(CbStrFind, FindStartPos) {
    cb_string_t* s = cb_str.create("aaa aaa aaa");
    size_t pos = cb_str.find(s, "a", 3); /* Start searching from position 3 (space), first 'a' after at 4 */
    EXPECT_EQ(pos, 4u);
    cb_str.free(s);
}

TEST(CbStrFind, FindEmptyStringFound) {
    cb_string_t* s = cb_str.create("Test");
    size_t pos = cb_str.find(s, "", 0); /* Empty string */
    EXPECT_EQ(pos, 0u);
    cb_str.free(s);
}

TEST(CbStrFind, FindFromPositionGreaterThanLength) {
    cb_string_t* s = cb_str.create("Test");
    size_t pos = cb_str.find(s, "e", 100); /* Start position > length */
    EXPECT_EQ(pos, CB_STR_NPOS);
    cb_str.free(s);
}

/* ============================================================================
 * Operation Tests - substr
 * ============================================================================ */

TEST(CbStrSubstr, SubstrBasic) {
    cb_string_t* s = cb_str.create("Hello World");
    cb_string_t* sub = cb_str.substr(s, 6, 5);
    EXPECT_STREQ("World", cb_str.c_str(sub));
    EXPECT_EQ(cb_str.length(sub), 5u);
    cb_str.free(sub);
    cb_str.free(s);
}

TEST(CbStrSubstr, SubstrFromBeginning) {
    cb_string_t* s = cb_str.create("Hello World");
    cb_string_t* sub = cb_str.substr(s, 0, 5);
    EXPECT_STREQ("Hello", cb_str.c_str(sub));
    cb_str.free(sub);
    cb_str.free(s);
}

TEST(CbStrSubstr, SubstrToEnd) {
    cb_string_t* s = cb_str.create("Hello World");
    cb_string_t* sub = cb_str.substr(s, 6, 100); /* More than remaining */
    EXPECT_STREQ("World", cb_str.c_str(sub));
    cb_str.free(sub);
    cb_str.free(s);
}

TEST(CbStrSubstr, SubstrInvalidPosition) {
    cb_string_t* s = cb_str.create("Test");
    cb_string_t* sub = cb_str.substr(s, 100, 5); /* Invalid position */
    EXPECT_TRUE(cb_str.empty(sub));
    cb_str.free(sub);
    cb_str.free(s);
}

TEST(CbStrSubstr, SubstrEmptyString) {
    cb_string_t* s = cb_str.create("");
    cb_string_t* sub = cb_str.substr(s, 0, 0);
    EXPECT_TRUE(cb_str.empty(sub));
    cb_str.free(sub);
    cb_str.free(s);
}

/* ============================================================================
 * Operation Tests - catch_in_range
 * ============================================================================ */

TEST(CbStrCatchInRange, CatchInRangeBasic) {
    cb_string_t* s = cb_str.create("Request: GET /api/users");
    cb_string_t* result = cb_str.catch_in_range(s, ": ", " ");
    EXPECT_STREQ("GET", cb_str.c_str(result));
    cb_str.free(result);
    cb_str.free(s);
}

TEST(CbStrCatchInRange, CatchInRangeNoStart) {
    cb_string_t* s = cb_str.create("Hello World");
    cb_string_t* result = cb_str.catch_in_range(s, "XX", "YY");
    EXPECT_TRUE(cb_str.empty(result));
    cb_str.free(result);
    cb_str.free(s);
}

TEST(CbStrCatchInRange, CatchInRangeNoEnd) {
    cb_string_t* s = cb_str.create("Hello World");
    cb_string_t* result = cb_str.catch_in_range(s, "Hello", "XXX");
    EXPECT_TRUE(cb_str.empty(result));
    cb_str.free(result);
    cb_str.free(s);
}

TEST(CbStrCatchInRange, CatchInRangeEmptyContent) {
    cb_string_t* s = cb_str.create("Start End");
    cb_string_t* result = cb_str.catch_in_range(s, "Start ", " End");
    EXPECT_TRUE(cb_str.empty(result));
    cb_str.free(result);
    cb_str.free(s);
}

/* ============================================================================
 * Split Tests
 * ============================================================================ */

TEST(CbStrSplit, SplitBasic) {
    cb_string_t* s = cb_str.create("one,two,three");
    cb_str_parts_t parts = cb_str.split(s, ",");

    EXPECT_EQ(parts.count, 3u);
    EXPECT_NE(parts.parts, nullptr);
    EXPECT_STREQ("one",   cb_str.c_str(parts.parts[0]));
    EXPECT_STREQ("two",   cb_str.c_str(parts.parts[1]));
    EXPECT_STREQ("three", cb_str.c_str(parts.parts[2]));

    cb_str_parts_free(&parts);
    cb_str.free(s);
}

TEST(CbStrSplit, SplitSingleToken) {
    cb_string_t* s = cb_str.create("no_delimiters");
    cb_str_parts_t parts = cb_str.split(s, ",");

    EXPECT_EQ(parts.count, 1u);
    EXPECT_STREQ("no_delimiters", cb_str.c_str(parts.parts[0]));

    cb_str_parts_free(&parts);
    cb_str.free(s);
}

TEST(CbStrSplit, SplitEmptyString) {
    cb_string_t* s = cb_str.create("");
    cb_str_parts_t parts = cb_str.split(s, ",");

    EXPECT_EQ(parts.count, 0u);
    EXPECT_EQ(parts.parts, nullptr);

    cb_str_parts_free(&parts);
    cb_str.free(s);
}

TEST(CbStrSplit, SplitNullDelimiter) {
    cb_string_t* s = cb_str.create("Test String");
    cb_str_parts_t parts = cb_str.split(s, NULL);

    EXPECT_EQ(parts.count, 1u);
    EXPECT_STREQ("Test String", cb_str.c_str(parts.parts[0]));

    cb_str_parts_free(&parts);
    cb_str.free(s);
}

TEST(CbStrSplit, SplitEmptyDelimiter) {
    cb_string_t* s = cb_str.create("Test");
    cb_str_parts_t parts = cb_str.split(s, "");

    EXPECT_EQ(parts.count, 1u);
    EXPECT_STREQ("Test", cb_str.c_str(parts.parts[0]));

    cb_str_parts_free(&parts);
    cb_str.free(s);
}

TEST(CbStrSplit, SplitMultipleDelimiters) {
    cb_string_t* s = cb_str.create("a,,b,c");
    cb_str_parts_t parts = cb_str.split(s, ",");

    EXPECT_EQ(parts.count, 4u);
    EXPECT_STREQ("a", cb_str.c_str(parts.parts[0]));
    EXPECT_STREQ("",  cb_str.c_str(parts.parts[1]));
    EXPECT_STREQ("b", cb_str.c_str(parts.parts[2]));
    EXPECT_STREQ("c", cb_str.c_str(parts.parts[3]));

    cb_str_parts_free(&parts);
    cb_str.free(s);
}

TEST(CbStrSplit, SplitTrailingDelimiter) {
    cb_string_t* s = cb_str.create("a,b,c,");
    cb_str_parts_t parts = cb_str.split(s, ",");

    EXPECT_EQ(parts.count, 4u);
    EXPECT_STREQ("a", cb_str.c_str(parts.parts[0]));
    EXPECT_STREQ("b", cb_str.c_str(parts.parts[1]));
    EXPECT_STREQ("c", cb_str.c_str(parts.parts[2]));
    EXPECT_STREQ("",  cb_str.c_str(parts.parts[3]));

    cb_str_parts_free(&parts);
    cb_str.free(s);
}

TEST(CbStrSplit, PartsFreeClearsStruct) {
    cb_string_t* s = cb_str.create("a,b");
    cb_str_parts_t parts = cb_str.split(s, ",");
    cb_str_parts_free(&parts);

    EXPECT_EQ(parts.count, 0u);
    EXPECT_EQ(parts.parts, nullptr);
    cb_str.free(s);
}

/* ============================================================================
 * Edge Cases & Stress Tests
 * ============================================================================ */

TEST(CbStrEdgeCases, LargeStringAllocation) {
    char large_buf[200];
    for (int i = 0; i < 199; i++)
        large_buf[i] = 'A' + (i % 26);
    large_buf[199] = '\0';

    cb_string_t* s = cb_str.create(large_buf);
    EXPECT_NE(s, nullptr);
    EXPECT_EQ(cb_str.length(s), 199u);
    EXPECT_STREQ(large_buf, cb_str.c_str(s));

    cb_str.free(s);
}

TEST(CbStrEdgeCases, RepeatedAppend) {
    cb_string_t* s = cb_str.create("A");
    for (int i = 0; i < 50; i++) {
        cb_str.append(s, "B");
    }
    EXPECT_EQ(cb_str.length(s), 51u);
    EXPECT_EQ(cb_str.c_str(s)[0], 'A');
    for (size_t i = 1; i < cb_str.length(s); i++) {
        EXPECT_EQ(cb_str.c_str(s)[i], 'B');
    }

    cb_str.free(s);
}

TEST(CbStrEdgeCases, NullStringOperations) {
    /* These operations should handle NULL gracefully */
    EXPECT_EQ(cb_str.length(NULL), 0u);
    EXPECT_TRUE(cb_str.empty(NULL));
    EXPECT_STREQ("", cb_str.c_str(NULL));
}

TEST(CbStrEdgeCases, EscapeChars) {
    cb_string_t* s = cb_str.create("Tab:\tNewline:\nBackslash:\\");
    EXPECT_EQ(cb_str.length(s), 25u);
    cb_str.free(s);
}

TEST(CbStrEdgeCases, MultidimensionalSpaceTrim) {
    cb_string_t* s = cb_str.create("  \t  \n  Trim Me  \n\t  ");
    cb_str.trim(s);
    EXPECT_STREQ("Trim Me", cb_str.c_str(s));
    cb_str.free(s);
}

TEST(CbStrEdgeCases, UnicodeFriendlyAscii) {
    cb_string_t* s = cb_str.create("ASCII only: 12345!");
    cb_str.append(s, " More text");
    EXPECT_STREQ("ASCII only: 12345! More text", cb_str.c_str(s));
    cb_str.free(s);
}
