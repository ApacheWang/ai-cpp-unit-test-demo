#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "string_utils.h" // module under test

// toUpper
TEST(StringUtilsTest, ToUpper_Normal) {
  EXPECT_EQ(StringUtils::toUpper(std::string("abc")), std::string("ABC"));
  EXPECT_EQ(StringUtils::toUpper(std::string("a1b2")), std::string("A1B2"));
}

TEST(StringUtilsTest, ToUpper_Boundary) {
  EXPECT_EQ(StringUtils::toUpper(std::string("")), std::string(""));
  EXPECT_EQ(StringUtils::toUpper(std::string("ABC")), std::string("ABC"));
  EXPECT_EQ(StringUtils::toUpper(std::string("123!")), std::string("123!"));
}

TEST(StringUtilsTest, ToUpper_Exception) {
  std::string longStr(100000, 'a');
  EXPECT_THROW(StringUtils::toUpper(longStr), std::invalid_argument);
}

// toLower
TEST(StringUtilsTest, ToLower_Normal) {
  EXPECT_EQ(StringUtils::toLower(std::string("ABC")), std::string("abc"));
  EXPECT_EQ(StringUtils::toLower(std::string("A1B2")), std::string("a1b2"));
}

TEST(StringUtilsTest, ToLower_Boundary) {
  EXPECT_EQ(StringUtils::toLower(std::string("")), std::string(""));
  EXPECT_EQ(StringUtils::toLower(std::string("abc")), std::string("abc"));
  EXPECT_EQ(StringUtils::toLower(std::string("123!")), std::string("123!"));
}

TEST(StringUtilsTest, ToLower_Exception) {
  std::string longStr(100000, 'b');
  EXPECT_THROW(StringUtils::toLower(longStr), std::invalid_argument);
}

// trim
TEST(StringUtilsTest, Trim_Normal) {
  EXPECT_EQ(StringUtils::trim(std::string("  a  ")), std::string("a"));
  EXPECT_EQ(StringUtils::trim(std::string("no_trim")), std::string("no_trim"));
}

TEST(StringUtilsTest, Trim_Boundary) {
  EXPECT_EQ(StringUtils::trim(std::string("")), std::string(""));
  EXPECT_EQ(StringUtils::trim(std::string("    ")), std::string(""));
}

TEST(StringUtilsTest, Trim_Exception) {
  std::string longStr(100000, ' ');
  EXPECT_THROW(StringUtils::trim(longStr), std::invalid_argument);
}

// trimLeft
TEST(StringUtilsTest, TrimLeft_Normal) {
  EXPECT_EQ(StringUtils::trimLeft(std::string("  abc")), std::string("abc"));
}

TEST(StringUtilsTest, TrimLeft_Boundary) {
  EXPECT_EQ(StringUtils::trimLeft(std::string("")), std::string(""));
  EXPECT_EQ(StringUtils::trimLeft(std::string("abc")), std::string("abc"));
}

TEST(StringUtilsTest, TrimLeft_Exception) {
  std::string longStr(100000, ' ');
  EXPECT_THROW(StringUtils::trimLeft(longStr), std::invalid_argument);
}

// trimRight
TEST(StringUtilsTest, TrimRight_Normal) {
  EXPECT_EQ(StringUtils::trimRight(std::string("abc  ")), std::string("abc"));
}

TEST(StringUtilsTest, TrimRight_Boundary) {
  EXPECT_EQ(StringUtils::trimRight(std::string("")), std::string(""));
  EXPECT_EQ(StringUtils::trimRight(std::string("abc")), std::string("abc"));
}

TEST(StringUtilsTest, TrimRight_Exception) {
  std::string longStr(100000, ' ');
  EXPECT_THROW(StringUtils::trimRight(longStr), std::invalid_argument);
}

// split
TEST(StringUtilsTest, Split_Normal) {
  std::vector<std::string> res = StringUtils::split(std::string("a,b,c"), std::string(","));
  ASSERT_EQ(res.size(), 3);
  EXPECT_EQ(res[0], std::string("a"));
  EXPECT_EQ(res[1], std::string("b"));
  EXPECT_EQ(res[2], std::string("c"));
}

TEST(StringUtilsTest, Split_Boundary) {
  std::vector<std::string> res = StringUtils::split(std::string("a|b|c"), std::string("|"));
  ASSERT_EQ(res.size(), 3);
  EXPECT_EQ(res[0], std::string("a"));
  EXPECT_EQ(res[1], std::string("b"));
  EXPECT_EQ(res[2], std::string("c"));
}

TEST(StringUtilsTest, Split_Exception) {
  EXPECT_THROW(StringUtils::split(std::string("abc"), std::string("")), std::invalid_argument);
}

// replace
TEST(StringUtilsTest, Replace_Normal) {
  EXPECT_EQ(StringUtils::replace(std::string("hello world"), std::string("world"), std::string("there")), std::string("hello there"));
}

TEST(StringUtilsTest, Replace_Boundary) {
  EXPECT_EQ(StringUtils::replace(std::string("aaaa"), std::string("aa"), std::string("b")), std::string("bb"));
}

TEST(StringUtilsTest, Replace_Exception) {
  EXPECT_THROW(StringUtils::replace(std::string("abc"), std::string(""), std::string("x")), std::invalid_argument);
}

// startsWith
TEST(StringUtilsTest, StartsWith_Normal) {
  EXPECT_TRUE(StringUtils::startsWith(std::string("abcdef"), std::string("abc")));
}

TEST(StringUtilsTest, StartsWith_Boundary) {
  EXPECT_TRUE(StringUtils::startsWith(std::string("abcdef"), std::string("")));
  EXPECT_FALSE(StringUtils::startsWith(std::string("abcdef"), std::string("z")));
}

TEST(StringUtilsTest, StartsWith_Exception) {
  EXPECT_THROW(StringUtils::startsWith(std::string("abcdef"), std::string("")), std::invalid_argument);
}

// endsWith
TEST(StringUtilsTest, EndsWith_Normal) {
  EXPECT_TRUE(StringUtils::endsWith(std::string("abcdef"), std::string("def")));
}

TEST(StringUtilsTest, EndsWith_Boundary) {
  EXPECT_TRUE(StringUtils::endsWith(std::string("abcdef"), std::string("")));
  EXPECT_FALSE(StringUtils::endsWith(std::string("abcdef"), std::string("g")));
}

TEST(StringUtilsTest, EndsWith_Exception) {
  EXPECT_THROW(StringUtils::endsWith(std::string("abcdef"), std::string("")), std::invalid_argument);
}

// reverse
TEST(StringUtilsTest, Reverse_Normal) {
  EXPECT_EQ(StringUtils::reverse(std::string("abc")), std::string("cba"));
}

TEST(StringUtilsTest, Reverse_Boundary) {
  EXPECT_EQ(StringUtils::reverse(std::string("")), std::string(""));
  EXPECT_EQ(StringUtils::reverse(std::string("a")), std::string("a"));
}

TEST(StringUtilsTest, Reverse_Exception) {
  std::string longStr(100000, 'a');
  EXPECT_THROW(StringUtils::reverse(longStr), std::invalid_argument);
}

// repeat
TEST(StringUtilsTest, Repeat_Normal) {
  EXPECT_EQ(StringUtils::repeat(std::string("ab"), 3), std::string("ababab"));
}

TEST(StringUtilsTest, Repeat_Boundary) {
  EXPECT_EQ(StringUtils::repeat(std::string("ab"), 0), std::string(""));
  EXPECT_EQ(StringUtils::repeat(std::string("a"), 3), std::string("aaa"));
}

TEST(StringUtilsTest, Repeat_Exception) {
  EXPECT_THROW(StringUtils::repeat(std::string("a"), -1), std::invalid_argument);
}

// camelToSnake
TEST(StringUtilsTest, CamelToSnake_Normal) {
  EXPECT_EQ(StringUtils::camelToSnake(std::string("camelCaseTest")), std::string("camel_case_test"));
}

TEST(StringUtilsTest, CamelToSnake_Boundary) {
  EXPECT_EQ(StringUtils::camelToSnake(std::string("")), std::string(""));
  EXPECT_EQ(StringUtils::camelToSnake(std::string("already_snake")), std::string("already_snake"));
}

TEST(StringUtilsTest, CamelToSnake_Exception) {
  EXPECT_THROW(StringUtils::camelToSnake(std::string("thisIsAVeryLongStringThatMightTriggerLimitsInSomeImplementationsJustToBeSafeAndTestExceptionHandling")), std::invalid_argument);
}
