/**
 * test_data_parser.cpp — AI 生成的 DataParser 单元测试
 *
 * 在演示时，可以删除本文件，然后在 OpenCode 中输入 prompt 重新生成。
 */

#include <gtest/gtest.h>
#include "data_parser.h"

// ============================================================
// parseKeyValue
// ============================================================

TEST(DataParserTest, ParseKeyValueBasic) {
    auto [key, value] = DataParser::parseKeyValue("name=OpenCode");
    EXPECT_EQ(key, "name");
    EXPECT_EQ(value, "OpenCode");
}

TEST(DataParserTest, ParseKeyValueWithSpaces) {
    auto [key, value] = DataParser::parseKeyValue("  key  =  value  ");
    EXPECT_EQ(key, "key");
    EXPECT_EQ(value, "value");
}

TEST(DataParserTest, ParseKeyValueEmptyValue) {
    auto [key, value] = DataParser::parseKeyValue("flag=");
    EXPECT_EQ(key, "flag");
    EXPECT_EQ(value, "");
}

TEST(DataParserTest, ParseKeyValueNoEquals) {
    EXPECT_THROW(DataParser::parseKeyValue("noequals"), std::invalid_argument);
}

TEST(DataParserTest, ParseKeyValueStartsWithEquals) {
    EXPECT_THROW(DataParser::parseKeyValue("=value"), std::invalid_argument);
}

TEST(DataParserTest, ParseKeyValueWithEqualsInValue) {
    auto [key, value] = DataParser::parseKeyValue("expr=1+2=3");
    EXPECT_EQ(key, "expr");
    EXPECT_EQ(value, "1+2=3");
}

// ============================================================
// parseIntList
// ============================================================

TEST(DataParserTest, ParseIntListBasic) {
    auto result = DataParser::parseIntList("1,2,3,4,5");
    ASSERT_EQ(result.size(), 5u);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[4], 5);
}

TEST(DataParserTest, ParseIntListWithSpaces) {
    auto result = DataParser::parseIntList(" 1 , 2 , 3 ");
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 2);
    EXPECT_EQ(result[2], 3);
}

TEST(DataParserTest, ParseIntListEmpty) {
    auto result = DataParser::parseIntList("");
    EXPECT_TRUE(result.empty());
}

TEST(DataParserTest, ParseIntListSkipInvalid) {
    auto result = DataParser::parseIntList("1,abc,3");
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 3);
}

TEST(DataParserTest, ParseIntListNegative) {
    auto result = DataParser::parseIntList("-1,0,1");
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], -1);
    EXPECT_EQ(result[2], 1);
}

// ============================================================
// toInt / toDouble / toBool
// ============================================================

TEST(DataParserTest, ToInt) {
    EXPECT_EQ(DataParser::toInt("42"), 42);
    EXPECT_EQ(DataParser::toInt("-7"), -7);
    EXPECT_EQ(DataParser::toInt("0"), 0);
    EXPECT_FALSE(DataParser::toInt("3.14").has_value());
    EXPECT_FALSE(DataParser::toInt("abc").has_value());
    EXPECT_FALSE(DataParser::toInt("").has_value());
    EXPECT_FALSE(DataParser::toInt("42abc").has_value()); // trailing chars
}

TEST(DataParserTest, ToDouble) {
    EXPECT_DOUBLE_EQ(DataParser::toDouble("3.14").value(), 3.14);
    EXPECT_DOUBLE_EQ(DataParser::toDouble("-0.5").value(), -0.5);
    EXPECT_DOUBLE_EQ(DataParser::toDouble("42").value(), 42.0);
    EXPECT_FALSE(DataParser::toDouble("abc").has_value());
    EXPECT_FALSE(DataParser::toDouble("").has_value());
}

TEST(DataParserTest, ToBool) {
    EXPECT_TRUE(DataParser::toBool("true").value());
    EXPECT_TRUE(DataParser::toBool("True").value());
    EXPECT_TRUE(DataParser::toBool("TRUE").value());
    EXPECT_TRUE(DataParser::toBool("1").value());
    EXPECT_TRUE(DataParser::toBool("yes").value());
    EXPECT_FALSE(DataParser::toBool("false").value());
    EXPECT_FALSE(DataParser::toBool("0").value());
    EXPECT_FALSE(DataParser::toBool("no").value());
    EXPECT_FALSE(DataParser::toBool("maybe").has_value());
}

// ============================================================
// parseIni
// ============================================================

TEST(DataParserTest, ParseIniBasic) {
    std::string ini = R"(
[database]
host=localhost
port=3306

[server]
port=8080
debug=true
)";
    auto result = DataParser::parseIni(ini);
    EXPECT_EQ(result["database"]["host"], "localhost");
    EXPECT_EQ(result["database"]["port"], "3306");
    EXPECT_EQ(result["server"]["port"], "8080");
    EXPECT_EQ(result["server"]["debug"], "true");
}

TEST(DataParserTest, ParseIniEmptyContent) {
    auto result = DataParser::parseIni("");
    EXPECT_TRUE(result.empty());
}

TEST(DataParserTest, ParseIniCommentsAndBlankLines) {
    std::string ini = R"(
; This is a comment
# Another comment

[section]
key=value
)";
    auto result = DataParser::parseIni(ini);
    EXPECT_EQ(result["section"]["key"], "value");
}

TEST(DataParserTest, ParseIniGlobalSection) {
    std::string ini = "key1=value1\nkey2=value2";
    auto result = DataParser::parseIni(ini);
    EXPECT_EQ(result[""]["key1"], "value1");
    EXPECT_EQ(result[""]["key2"], "value2");
}

// ============================================================
// parseValue
// ============================================================

TEST(DataParserTest, ParseValueBool) {
    auto v1 = DataParser::parseValue("true");
    EXPECT_TRUE(std::holds_alternative<bool>(v1));
    EXPECT_TRUE(std::get<bool>(v1));

    auto v2 = DataParser::parseValue("false");
    EXPECT_TRUE(std::holds_alternative<bool>(v2));
    EXPECT_FALSE(std::get<bool>(v2));
}

TEST(DataParserTest, ParseValueInt) {
    auto v = DataParser::parseValue("42");
    EXPECT_TRUE(std::holds_alternative<int>(v));
    EXPECT_EQ(std::get<int>(v), 42);
}

TEST(DataParserTest, ParseValueDouble) {
    auto v = DataParser::parseValue("3.14");
    EXPECT_TRUE(std::holds_alternative<double>(v));
    EXPECT_DOUBLE_EQ(std::get<double>(v), 3.14);
}

TEST(DataParserTest, ParseValueString) {
    auto v1 = DataParser::parseValue("\"hello world\"");
    EXPECT_TRUE(std::holds_alternative<std::string>(v1));
    EXPECT_EQ(std::get<std::string>(v1), "hello world");

    auto v2 = DataParser::parseValue("'quoted'");
    EXPECT_TRUE(std::holds_alternative<std::string>(v2));
    EXPECT_EQ(std::get<std::string>(v2), "quoted");
}

TEST(DataParserTest, ParseValueNull) {
    auto v = DataParser::parseValue("null");
    EXPECT_TRUE(std::holds_alternative<std::string>(v));
    EXPECT_EQ(std::get<std::string>(v), "");
}

TEST(DataParserTest, ParseValueUnknownFallback) {
    auto v = DataParser::parseValue("unknown_token");
    EXPECT_TRUE(std::holds_alternative<std::string>(v));
    EXPECT_EQ(std::get<std::string>(v), "unknown_token");
}

// ============================================================
// escape / unescape
// ============================================================

TEST(DataParserTest, Escape) {
    EXPECT_EQ(DataParser::escape("hello\"world"), "hello\\\"world");
    EXPECT_EQ(DataParser::escape("a\\b"), "a\\\\b");
    EXPECT_EQ(DataParser::escape("line1\nline2"), "line1\\nline2");
    EXPECT_EQ(DataParser::escape("tab\there"), "tab\\there");
    EXPECT_EQ(DataParser::escape("normal"), "normal");
}

TEST(DataParserTest, Unescape) {
    EXPECT_EQ(DataParser::unescape("hello\\\"world"), "hello\"world");
    EXPECT_EQ(DataParser::unescape("a\\\\b"), "a\\b");
    EXPECT_EQ(DataParser::unescape("line1\\nline2"), "line1\nline2");
    EXPECT_EQ(DataParser::unescape("tab\\there"), "tab\there");
}

TEST(DataParserTest, EscapeUnescapeRoundtrip) {
    std::string original = "Hello\t\"World\"\nNew\\Line";
    EXPECT_EQ(DataParser::unescape(DataParser::escape(original)), original);
}
