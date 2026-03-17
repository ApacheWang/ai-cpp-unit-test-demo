/**
 * test_string_utils.cpp — AI 生成的 StringUtils 单元测试
 *
 * 在演示时，可以删除本文件，然后在 OpenCode 中输入 prompt 重新生成。
 */

#include <gtest/gtest.h>
#include "string_utils.h"

// ============================================================
// toUpper / toLower
// ============================================================

TEST(StringUtilsTest, ToUpperBasic) {
    EXPECT_EQ(StringUtils::toUpper("hello"), "HELLO");
    EXPECT_EQ(StringUtils::toUpper("Hello World"), "HELLO WORLD");
    EXPECT_EQ(StringUtils::toUpper(""), "");
    EXPECT_EQ(StringUtils::toUpper("123"), "123");
    EXPECT_EQ(StringUtils::toUpper("ALREADY"), "ALREADY");
}

TEST(StringUtilsTest, ToLowerBasic) {
    EXPECT_EQ(StringUtils::toLower("HELLO"), "hello");
    EXPECT_EQ(StringUtils::toLower("Hello World"), "hello world");
    EXPECT_EQ(StringUtils::toLower(""), "");
    EXPECT_EQ(StringUtils::toLower("123"), "123");
    EXPECT_EQ(StringUtils::toLower("already"), "already");
}

TEST(StringUtilsTest, CaseConversionRoundtrip) {
    // toUpper(toLower(str)) 和 toLower(toUpper(str)) 应该一致（全小写/全大写）
    std::string original = "Hello World 123";
    EXPECT_EQ(StringUtils::toUpper(StringUtils::toLower(original)),
              StringUtils::toUpper(original));
}

// ============================================================
// trim / trimLeft / trimRight
// ============================================================

TEST(StringUtilsTest, TrimBasic) {
    EXPECT_EQ(StringUtils::trim("  hello  "), "hello");
    EXPECT_EQ(StringUtils::trim("\thello\n"), "hello");
    EXPECT_EQ(StringUtils::trim("hello"), "hello");
    EXPECT_EQ(StringUtils::trim("   "), "");
    EXPECT_EQ(StringUtils::trim(""), "");
}

TEST(StringUtilsTest, TrimLeft) {
    EXPECT_EQ(StringUtils::trimLeft("  hello"), "hello");
    EXPECT_EQ(StringUtils::trimLeft("hello  "), "hello  ");
    EXPECT_EQ(StringUtils::trimLeft("  "), "");
}

TEST(StringUtilsTest, TrimRight) {
    EXPECT_EQ(StringUtils::trimRight("hello  "), "hello");
    EXPECT_EQ(StringUtils::trimRight("  hello"), "  hello");
    EXPECT_EQ(StringUtils::trimRight("  "), "");
}

// ============================================================
// split
// ============================================================

TEST(StringUtilsTest, SplitBasic) {
    auto result = StringUtils::split("a,b,c", ",");
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "b");
    EXPECT_EQ(result[2], "c");
}

TEST(StringUtilsTest, SplitMultiCharDelimiter) {
    auto result = StringUtils::split("a::b::c", "::");
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "b");
    EXPECT_EQ(result[2], "c");
}

TEST(StringUtilsTest, SplitEmptyInput) {
    auto result = StringUtils::split("", ",");
    EXPECT_TRUE(result.empty());
}

TEST(StringUtilsTest, SplitKeepEmpty) {
    auto result = StringUtils::split("a,,b", ",", true);
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "");
    EXPECT_EQ(result[2], "b");
}

TEST(StringUtilsTest, SplitSkipEmpty) {
    auto result = StringUtils::split("a,,b", ",", false);
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[1], "b");
}

// ============================================================
// replace
// ============================================================

TEST(StringUtilsTest, ReplaceBasic) {
    EXPECT_EQ(StringUtils::replace("hello world", "world", "earth"), "hello earth");
    EXPECT_EQ(StringUtils::replace("aaa", "a", "b"), "bbb");
    EXPECT_EQ(StringUtils::replace("hello", "x", "y"), "hello"); // no match
}

TEST(StringUtilsTest, ReplaceOverlapping) {
    EXPECT_EQ(StringUtils::replace("aaa", "aa", "b"), "ba");
}

TEST(StringUtilsTest, ReplaceEmptyFrom) {
    EXPECT_EQ(StringUtils::replace("abc", "", "x"), "abc");
}

// ============================================================
// startsWith / endsWith
// ============================================================

TEST(StringUtilsTest, StartsWith) {
    EXPECT_TRUE(StringUtils::startsWith("hello world", "hello"));
    EXPECT_TRUE(StringUtils::startsWith("hello", "hello"));
    EXPECT_FALSE(StringUtils::startsWith("hello", "world"));
    EXPECT_FALSE(StringUtils::startsWith("hi", "hello")); // prefix longer than str
    EXPECT_TRUE(StringUtils::startsWith("", ""));
}

TEST(StringUtilsTest, EndsWith) {
    EXPECT_TRUE(StringUtils::endsWith("hello world", "world"));
    EXPECT_TRUE(StringUtils::endsWith("hello", "hello"));
    EXPECT_FALSE(StringUtils::endsWith("hello", "he"));
    EXPECT_FALSE(StringUtils::endsWith("hi", "hello")); // suffix longer
    EXPECT_TRUE(StringUtils::endsWith("", ""));
}

// ============================================================
// reverse / repeat
// ============================================================

TEST(StringUtilsTest, Reverse) {
    EXPECT_EQ(StringUtils::reverse("hello"), "olleh");
    EXPECT_EQ(StringUtils::reverse("a"), "a");
    EXPECT_EQ(StringUtils::reverse(""), "");
    EXPECT_EQ(StringUtils::reverse("12345"), "54321");
}

TEST(StringUtilsTest, ReverseDoubleReverse) {
    std::string original = "OpenCode Demo";
    EXPECT_EQ(StringUtils::reverse(StringUtils::reverse(original)), original);
}

TEST(StringUtilsTest, Repeat) {
    EXPECT_EQ(StringUtils::repeat("ab", 3), "ababab");
    EXPECT_EQ(StringUtils::repeat("x", 1), "x");
    EXPECT_EQ(StringUtils::repeat("x", 0), "");
    EXPECT_EQ(StringUtils::repeat("x", -1), "");
}

// ============================================================
// camelToSnake
// ============================================================

TEST(StringUtilsTest, CamelToSnake) {
    EXPECT_EQ(StringUtils::camelToSnake("helloWorld"), "hello_world");
    EXPECT_EQ(StringUtils::camelToSnake("camelCase"), "camel_case");
    EXPECT_EQ(StringUtils::camelToSnake("XMLParser"), "x_m_l_parser");
    EXPECT_EQ(StringUtils::camelToSnake("a"), "a");
    EXPECT_EQ(StringUtils::camelToSnake(""), "");
}
