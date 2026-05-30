/**
 * @file str_test.cpp
 * @author Florent ALBANY - FAL
 * @brief Comprehensive test suite for CTool string utilities (ctool::str) using Google Test.
 * @version 1.0
 * @date 2026-05-29
 */

#include <gtest/gtest.h>
#include "../../ctool/string.hpp"
#include <string>
#include <vector>

/* ============================================================================
 * assign Tests
 * ============================================================================ */

TEST(CtStrAssign, AssignNormal) {
    std::string s = "old";
    ctool::str::assign(s, "hello");
    EXPECT_EQ(s, "hello");
}

TEST(CtStrAssign, AssignEmpty) {
    std::string s = "old";
    ctool::str::assign(s, "");
    EXPECT_EQ(s, "");
}

TEST(CtStrAssign, AssignNullClears) {
    std::string s = "existing";
    ctool::str::assign(s, nullptr);
    EXPECT_EQ(s, "");
    EXPECT_TRUE(s.empty());
}

TEST(CtStrAssign, AssignNullToEmpty) {
    std::string s;
    ctool::str::assign(s, nullptr);
    EXPECT_EQ(s, "");
    EXPECT_TRUE(s.empty());
}

TEST(CtStrAssign, AssignOverwritesPrevious) {
    std::string s = "first";
    ctool::str::assign(s, "first");
    EXPECT_EQ(s, "first");
    ctool::str::assign(s, "second");
    EXPECT_EQ(s, "second");
    ctool::str::assign(s, nullptr);
    EXPECT_EQ(s, "");
}

/* ============================================================================
 * updateField Tests
 * ============================================================================ */

TEST(CtStrUpdateField, UpdateExistingField) {
    /* Token "SN001" starts with "SN" → updated to "SN999" */
    std::string result = ctool::str::updateField("SN001_LOT42", "SN", "999");
    EXPECT_EQ(result, "SN999_LOT42");
}

TEST(CtStrUpdateField, AppendNewField) {
    /* No token starts with "T" → appended */
    std::string result = ctool::str::updateField("SN001_LOT42", "T", "25.00");
    EXPECT_EQ(result, "SN001_LOT42_T25.00");
}

TEST(CtStrUpdateField, AppendToEmpty) {
    std::string result = ctool::str::updateField("", "T", "25.00");
    EXPECT_EQ(result, "T25.00");
}

TEST(CtStrUpdateField, UpdateAllMatchingTokens) {
    /* Both tokens start with "X" → both updated */
    std::string result = ctool::str::updateField("Xaaa_Xbbb", "X", "Y");
    EXPECT_EQ(result, "XY_XY");
}

TEST(CtStrUpdateField, EmptyPrefixMatchesAll) {
    /* Empty prefix: compare(0, 0, "") == 0 for all tokens → all become "" + newValue */
    std::string result = ctool::str::updateField("aaa_bbb", "", "X");
    EXPECT_EQ(result, "X_X");
}

TEST(CtStrUpdateField, MiddleMatchNotUpdated) {
    /* Token "Xaaa" does not start with "aa" → not matched, appended */
    std::string result = ctool::str::updateField("Xaaa", "aa", "Y");
    EXPECT_EQ(result, "Xaaa_aaY");
}

TEST(CtStrUpdateField, EmptyNewValueReplacesToken) {
    /* "PREFIX123" starts with "PREFIX", replaced with "PREFIX" + "" = "PREFIX" */
    std::string result = ctool::str::updateField("PREFIX123_val", "PREFIX", "");
    EXPECT_EQ(result, "PREFIX_val");
}

TEST(CtStrUpdateField, EmptyNewValueAppendsField) {
    /* No token starts with "C", so "C" + "" = "C" is appended */
    std::string result = ctool::str::updateField("A_B", "C", "");
    EXPECT_EQ(result, "A_B_C");
}

TEST(CtStrUpdateField, CustomDelimiter) {
    std::string result = ctool::str::updateField("SN001|LOT42", "LOT", "99", '|');
    EXPECT_EQ(result, "SN001|LOT99");
}

TEST(CtStrUpdateField, SingleTokenMatches) {
    std::string result = ctool::str::updateField("FOO", "FOO", "bar");
    EXPECT_EQ(result, "FOObar");
}

TEST(CtStrUpdateField, SingleTokenNoMatch) {
    std::string result = ctool::str::updateField("BAZ", "FOO", "bar");
    EXPECT_EQ(result, "BAZ_FOObar");
}

/* ============================================================================
 * findAndReplace Tests
 * ============================================================================ */

TEST(CtStrFindAndReplace, SingleOccurrence) {
    std::string result = ctool::str::findAndReplace("hello world", "world", "there");
    EXPECT_EQ(result, "hello there");
}

TEST(CtStrFindAndReplace, AllOccurrences) {
    std::string result = ctool::str::findAndReplace("ababab", "a", "X");
    EXPECT_EQ(result, "XbXbXb");
}

TEST(CtStrFindAndReplace, NoMatchReturnsOriginal) {
    std::string result = ctool::str::findAndReplace("hello", "xyz", "abc");
    EXPECT_EQ(result, "hello");
}

TEST(CtStrFindAndReplace, EmptyInput) {
    std::string result = ctool::str::findAndReplace("", "a", "b");
    EXPECT_EQ(result, "");
}

TEST(CtStrFindAndReplace, EmptySearchStr) {
    std::string result = ctool::str::findAndReplace("hello", "", "x");
    EXPECT_EQ(result, "hello");
}

TEST(CtStrFindAndReplace, EmptyReplaceStrRemoves) {
    std::string result = ctool::str::findAndReplace("hello", "l", "");
    EXPECT_EQ(result, "heo");
}

TEST(CtStrFindAndReplace, SearchEqualsReplaceIsNoOp) {
    std::string result = ctool::str::findAndReplace("hello", "l", "l");
    EXPECT_EQ(result, "hello");
}

TEST(CtStrFindAndReplace, NoOverlapReplace) {
    /* "aaa" with search="aa", replace="X": first "aa" at 0 → "Xa", pos=1, find from 1 → none */
    std::string result = ctool::str::findAndReplace("aaa", "aa", "X");
    EXPECT_EQ(result, "Xa");
}

TEST(CtStrFindAndReplace, ReplaceAtStart) {
    std::string result = ctool::str::findAndReplace("hello", "he", "HE");
    EXPECT_EQ(result, "HEllo");
}

TEST(CtStrFindAndReplace, ReplaceAtEnd) {
    std::string result = ctool::str::findAndReplace("hello", "lo", "LO");
    EXPECT_EQ(result, "helLO");
}

TEST(CtStrFindAndReplace, ReplaceFullString) {
    std::string result = ctool::str::findAndReplace("hello", "hello", "bye");
    EXPECT_EQ(result, "bye");
}

/* ============================================================================
 * findAndReplaceRecursive Tests
 * ============================================================================ */

TEST(CtStrFindAndReplaceRecursive, BasicReplace) {
    std::string result = ctool::str::findAndReplaceRecursive("hello world", "world", "there");
    EXPECT_EQ(result, "hello there");
}

TEST(CtStrFindAndReplaceRecursive, ReplaceAll) {
    std::string result = ctool::str::findAndReplaceRecursive("ababab", "a", "X");
    EXPECT_EQ(result, "XbXbXb");
}

TEST(CtStrFindAndReplaceRecursive, GuardPreventsInfiniteLoop) {
    /* replaceStr contains searchStr → guard triggers, returns original unchanged */
    testing::internal::CaptureStderr();
    std::string result        = ctool::str::findAndReplaceRecursive("hello", "a", "aa");
    std::string stderr_output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(result, "hello");
    EXPECT_FALSE(stderr_output.empty()) << "Should have logged error to stderr";
}

TEST(CtStrFindAndReplaceRecursive, EmptyInput) {
    std::string result = ctool::str::findAndReplaceRecursive("", "a", "b");
    EXPECT_EQ(result, "");
}

TEST(CtStrFindAndReplaceRecursive, EmptySearchStr) {
    std::string result = ctool::str::findAndReplaceRecursive("hello", "", "x");
    EXPECT_EQ(result, "hello");
}

TEST(CtStrFindAndReplaceRecursive, EmptyReplaceStrRemovesAll) {
    std::string result = ctool::str::findAndReplaceRecursive("hello", "l", "");
    EXPECT_EQ(result, "heo");
}

TEST(CtStrFindAndReplaceRecursive, GuardDoesNotTriggerForSafeArgs) {
    /* replaceStr does NOT contain searchStr → proceeds normally */
    testing::internal::CaptureStderr();
    std::string result        = ctool::str::findAndReplaceRecursive("hello", "l", "X");
    std::string stderr_output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(result, "heXXo");
    EXPECT_TRUE(stderr_output.empty()) << "No error should be logged for safe arguments";
}

TEST(CtStrFindAndReplaceRecursive, GuardsOnEmptyReplaceStrContainingSearch) {
    /* Empty string does not contain any non-empty search string, so guard passes.
     * Then since replaceStr is empty, searchStr is effectively removed. */
    std::string result = ctool::str::findAndReplaceRecursive("hello", "l", "");
    EXPECT_EQ(result, "heo");
}

/* ============================================================================
 * catchInRange Tests
 * ============================================================================ */

TEST(CtStrCatchInRange, BasicExtraction) {
    std::string result = ctool::str::catchInRange("before[content]after", "[", "]");
    EXPECT_EQ(result, "content");
}

TEST(CtStrCatchInRange, FirstDelimiterNotFound) {
    std::string result = ctool::str::catchInRange("hello world", "[", "]");
    EXPECT_EQ(result, "");
}

TEST(CtStrCatchInRange, SecondDelimiterNotFound) {
    std::string result = ctool::str::catchInRange("hello[world", "[", "]");
    EXPECT_EQ(result, "");
}

TEST(CtStrCatchInRange, NeitherDelimiterFound) {
    std::string result = ctool::str::catchInRange("hello", "[", "]");
    EXPECT_EQ(result, "");
}

TEST(CtStrCatchInRange, EmptyInput) {
    std::string result = ctool::str::catchInRange("", "a", "b");
    EXPECT_EQ(result, "");
}

TEST(CtStrCatchInRange, IdenticalDelimiters) {
    /* First "<" at 0, startPos=1. Find next "<" from 1 → at 6. substr(1,5) = "hello" */
    std::string result = ctool::str::catchInRange("<hello<world", "<", "<");
    EXPECT_EQ(result, "hello");
}

TEST(CtStrCatchInRange, DelimitersAtEdges) {
    std::string result = ctool::str::catchInRange("[content]", "[", "]");
    EXPECT_EQ(result, "content");
}

TEST(CtStrCatchInRange, EmptyContentBetweenDelimiters) {
    std::string result = ctool::str::catchInRange("[]", "[", "]");
    EXPECT_EQ(result, "");
}

TEST(CtStrCatchInRange, MultipleOccurrencesFirstIsUsed) {
    /* Uses first occurrence of each delimiter */
    std::string result = ctool::str::catchInRange("a[first]b[second]c", "[", "]");
    EXPECT_EQ(result, "first");
}

TEST(CtStrCatchInRange, MultiCharDelimiters) {
    std::string result = ctool::str::catchInRange("<<<content>>>", "<<<", ">>>");
    EXPECT_EQ(result, "content");
}

/* ============================================================================
 * split Tests
 * ============================================================================ */

TEST(CtStrSplit, BasicSplit) {
    std::vector<std::string> tokens = ctool::str::split("a,b,c", ',');
    ASSERT_EQ(tokens.size(), 3u);
    EXPECT_EQ(tokens[0], "a");
    EXPECT_EQ(tokens[1], "b");
    EXPECT_EQ(tokens[2], "c");
}

TEST(CtStrSplit, NoDelimiter) {
    std::vector<std::string> tokens = ctool::str::split("hello", ',');
    ASSERT_EQ(tokens.size(), 1u);
    EXPECT_EQ(tokens[0], "hello");
}

TEST(CtStrSplit, ConsecutiveDelimiters) {
    /* Empty tokens are discarded: "a,,b" → ["a", "b"] */
    std::vector<std::string> tokens = ctool::str::split("a,,b", ',');
    ASSERT_EQ(tokens.size(), 2u);
    EXPECT_EQ(tokens[0], "a");
    EXPECT_EQ(tokens[1], "b");
}

TEST(CtStrSplit, LeadingDelimiter) {
    /* Leading empty discarded: ",a,b" → ["a", "b"] */
    std::vector<std::string> tokens = ctool::str::split(",a,b", ',');
    ASSERT_EQ(tokens.size(), 2u);
    EXPECT_EQ(tokens[0], "a");
    EXPECT_EQ(tokens[1], "b");
}

TEST(CtStrSplit, TrailingDelimiter) {
    /* Trailing empty discarded: "a,b," → ["a", "b"] */
    std::vector<std::string> tokens = ctool::str::split("a,b,", ',');
    ASSERT_EQ(tokens.size(), 2u);
    EXPECT_EQ(tokens[0], "a");
    EXPECT_EQ(tokens[1], "b");
}

TEST(CtStrSplit, OnlyDelimiters) {
    /* All tokens empty, all discarded → empty result */
    std::vector<std::string> tokens = ctool::str::split(",,", ',');
    EXPECT_EQ(tokens.size(), 0u);
}

TEST(CtStrSplit, EmptyString) {
    std::vector<std::string> tokens = ctool::str::split("", ',');
    EXPECT_EQ(tokens.size(), 0u);
}

TEST(CtStrSplit, SingleCharacter) {
    std::vector<std::string> tokens = ctool::str::split("x", ',');
    ASSERT_EQ(tokens.size(), 1u);
    EXPECT_EQ(tokens[0], "x");
}

TEST(CtStrSplit, CustomDelimiter) {
    std::vector<std::string> tokens = ctool::str::split("a|b|c", '|');
    ASSERT_EQ(tokens.size(), 3u);
    EXPECT_EQ(tokens[0], "a");
    EXPECT_EQ(tokens[1], "b");
    EXPECT_EQ(tokens[2], "c");
}

TEST(CtStrSplit, NoEmptyTokensReturned) {
    /* Verify behavioral contract: empty tokens are discarded */
    std::vector<std::string> tokens = ctool::str::split("a,,,b", ',');
    ASSERT_EQ(tokens.size(), 2u);
    EXPECT_EQ(tokens[0], "a");
    EXPECT_EQ(tokens[1], "b");
}

/* ============================================================================
 * trim Tests
 * ============================================================================ */

TEST(CtStrTrim, TrimLeadingSpaces) {
    std::string result = ctool::str::trim("  hello");
    EXPECT_EQ(result, "hello");
}

TEST(CtStrTrim, TrimTrailingSpaces) {
    std::string result = ctool::str::trim("hello  ");
    EXPECT_EQ(result, "hello");
}

TEST(CtStrTrim, TrimBothSides) {
    std::string result = ctool::str::trim("  hello  ");
    EXPECT_EQ(result, "hello");
}

TEST(CtStrTrim, NoWhitespace) {
    std::string result = ctool::str::trim("hello");
    EXPECT_EQ(result, "hello");
}

TEST(CtStrTrim, AllWhitespace) {
    std::string result = ctool::str::trim("   ");
    EXPECT_EQ(result, "");
}

TEST(CtStrTrim, EmptyString) {
    std::string result = ctool::str::trim("");
    EXPECT_EQ(result, "");
}

TEST(CtStrTrim, TabAndNewline) {
    std::string result = ctool::str::trim("\t\nhello\r\n");
    EXPECT_EQ(result, "hello");
}

TEST(CtStrTrim, MixedWhitespaceTypes) {
    std::string result = ctool::str::trim(" \t\n\r\f\vhello \t\n\r\f\v");
    EXPECT_EQ(result, "hello");
}

TEST(CtStrTrim, WhitespaceOnlyInMiddle) {
    std::string result = ctool::str::trim("hello world");
    EXPECT_EQ(result, "hello world");
}

TEST(CtStrTrim, SingleCharacter) {
    std::string result = ctool::str::trim("a");
    EXPECT_EQ(result, "a");
}

TEST(CtStrTrim, SingleWhitespace) {
    std::string result = ctool::str::trim(" ");
    EXPECT_EQ(result, "");
}

/* ============================================================================
 * toString Tests
 * ============================================================================ */

TEST(CtStrToString, Int) {
    std::string result = ctool::str::toString(42);
    EXPECT_EQ(result, "42");
}

TEST(CtStrToString, NegativeInt) {
    std::string result = ctool::str::toString(-42);
    EXPECT_EQ(result, "-42");
}

TEST(CtStrToString, Zero) {
    std::string result = ctool::str::toString(0);
    EXPECT_EQ(result, "0");
}

TEST(CtStrToString, Double) {
    std::string result = ctool::str::toString(3.14);
    /* ostringstream default precision: 3.14 → "3.14" */
    EXPECT_EQ(result, "3.14");
}

TEST(CtStrToString, BoolTrue) {
    std::string result = ctool::str::toString(true);
    /* ostringstream << bool → "1" (not "true") */
    EXPECT_EQ(result, "1");
}

TEST(CtStrToString, BoolFalse) {
    std::string result = ctool::str::toString(false);
    EXPECT_EQ(result, "0");
}

TEST(CtStrToString, Char) {
    std::string result = ctool::str::toString('A');
    /* ostringstream << char → "A" */
    EXPECT_EQ(result, "A");
}

TEST(CtStrToString, ConstCharPtr) {
    std::string result = ctool::str::toString("hello");
    EXPECT_EQ(result, "hello");
}

TEST(CtStrToString, Float) {
    std::string result = ctool::str::toString(1.5f);
    /* ostringstream may print "1.5" for float */
    EXPECT_EQ(result, "1.5");
}

TEST(CtStrToString, LargeInt) {
    int         large  = 1234567890;
    std::string result = ctool::str::toString(large);
    EXPECT_EQ(result, "1234567890");
}

/* ============================================================================
 * Cross-Function & Edge Cases
 * ============================================================================ */

TEST(CtStrEdgeCases, TrimThenSplit) {
    std::string              trimmed = ctool::str::trim("  a,b,c  ");
    std::vector<std::string> tokens  = ctool::str::split(trimmed, ',');
    ASSERT_EQ(tokens.size(), 3u);
    EXPECT_EQ(tokens[0], "a");
    EXPECT_EQ(tokens[1], "b");
    EXPECT_EQ(tokens[2], "c");
}

TEST(CtStrEdgeCases, UpdateFieldThenSplit) {
    std::string              updated = ctool::str::updateField("A_B", "C", "D");
    std::vector<std::string> tokens  = ctool::str::split(updated, '_');
    ASSERT_EQ(tokens.size(), 3u);
    EXPECT_EQ(tokens[0], "A");
    EXPECT_EQ(tokens[1], "B");
    EXPECT_EQ(tokens[2], "CD");
}

TEST(CtStrEdgeCases, LargeStringSplit) {
    std::string big;
    for (int i = 0; i < 1000; ++i) {
        big += std::to_string(i) + ",";
    }
    big.pop_back(); /* remove trailing comma */
    std::vector<std::string> tokens = ctool::str::split(big, ',');
    EXPECT_EQ(tokens.size(), 1000u);
    EXPECT_EQ(tokens[0], "0");
    EXPECT_EQ(tokens[999], "999");
}

TEST(CtStrEdgeCases, FindAndReplaceInTrimmed) {
    std::string s        = "  foo bar foo  ";
    std::string trimmed  = ctool::str::trim(s);
    std::string replaced = ctool::str::findAndReplace(trimmed, "foo", "baz");
    EXPECT_EQ(replaced, "baz bar baz");
}

TEST(CtStrEdgeCases, CatchInRangeChained) {
    /* Extract version string between brackets, then split on '.' */
    std::string extracted = ctool::str::catchInRange("version[1.2.3]", "[", "]");
    EXPECT_EQ(extracted, "1.2.3");
    std::vector<std::string> parts = ctool::str::split(extracted, '.');
    ASSERT_EQ(parts.size(), 3u);
    EXPECT_EQ(parts[0], "1");
    EXPECT_EQ(parts[1], "2");
    EXPECT_EQ(parts[2], "3");
}

TEST(CtStrEdgeCases, AssignNullThenFindAndReplace) {
    std::string s = "hello";
    ctool::str::assign(s, nullptr);
    EXPECT_TRUE(s.empty());
    std::string result = ctool::str::findAndReplace(s, "a", "b");
    EXPECT_EQ(result, "");
}

TEST(CtStrEdgeCases, TrimIsIdempotent) {
    std::string input  = " \t\n hello world \n\t ";
    std::string first  = ctool::str::trim(input);
    std::string second = ctool::str::trim(first);
    EXPECT_EQ(first, second) << "trim() should be idempotent";
    EXPECT_EQ(first, "hello world");
}

TEST(CtStrEdgeCases, UpdateFieldWithEmptyFieldsFromSplit) {
    /* split("__", '_') returns empty vector (all empties discarded) → found=false → appended */
    std::string result = ctool::str::updateField("__", "P", "V");
    /* split("__") returns []. found=false. result="" → no delimiter prepended. result = "PV" */
    EXPECT_EQ(result, "PV");
}

TEST(CtStrEdgeCases, VeryLongStringTrim) {
    std::string spaces(10000, ' ');
    std::string middle = "x";
    std::string input  = spaces + middle + spaces;
    std::string result = ctool::str::trim(input);
    EXPECT_EQ(result, "x");
}

TEST(CtStrEdgeCases, FindAndReplaceWithEmptyReplaceStrRemovesAll) {
    std::string result = ctool::str::findAndReplace("aaaa", "a", "");
    EXPECT_EQ(result, "");
}

TEST(CtStrEdgeCases, CatchInRangeWithEmptyDelimiters) {
    /* Empty searchStr1: find("") returns 0. startPos = 0 + 0 = 0.
     * Empty searchStr2: find("", 0) returns 0. substr(0, 0) = "" */
    std::string result = ctool::str::catchInRange("hello", "", "");
    EXPECT_EQ(result, "");
}

TEST(CtStrEdgeCases, UpdateFieldAppendsToExisting) {
    /* "A_B" with no match for "C" → result = "A_B" (no change to tokens) + "_" + "C_val" */
    std::string result = ctool::str::updateField("A_B", "C", "val");
    EXPECT_EQ(result, "A_B_Cval");
}
