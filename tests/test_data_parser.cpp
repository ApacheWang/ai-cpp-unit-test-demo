#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include "data_parser.h"

// ==================== parseKeyValue ====================

TEST(DataParserTest, ParseKeyValue_Normal) {
    // 正常：简单键值对
    auto [key, value] = DataParser::parseKeyValue("name=John");
    EXPECT_EQ(key, "name");
    EXPECT_EQ(value, "John");
}

TEST(DataParserTest, ParseKeyValue_Boundary) {
    // 边界：值中包含等号，应以首个 '=' 分割
    auto [kv1, vv1] = DataParser::parseKeyValue("key=val=ue");
    EXPECT_EQ(kv1, "key");
    EXPECT_EQ(vv1, "val=ue");

    // 边界：值为空
    auto [kv2, vv2] = DataParser::parseKeyValue("key=");
    EXPECT_EQ(kv2, "key");
    EXPECT_EQ(vv2, "");
}

TEST(DataParserTest, ParseKeyValue_Error) {
    // 异常：无等号
    EXPECT_THROW(DataParser::parseKeyValue("no_equals"), std::invalid_argument);
    // 异常：等号在最前面
    EXPECT_THROW(DataParser::parseKeyValue("=no_key"), std::invalid_argument);
}

// ==================== parseIntList ====================

TEST(DataParserTest, ParseIntList_Normal) {
    auto result = DataParser::parseIntList("1,2,3");
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 2);
    EXPECT_EQ(result[2], 3);
}

TEST(DataParserTest, ParseIntList_Boundary) {
    // 空字符串
    EXPECT_TRUE(DataParser::parseIntList("").empty());
    // 带空格
    auto result = DataParser::parseIntList(" 1 , 2 , 3 ");
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0], 1);
}

TEST(DataParserTest, ParseIntList_SkipInvalid) {
    // 包含非数字项应跳过（不抛异常）
    auto result = DataParser::parseIntList("1,abc,3");
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 3);
}

// ==================== toInt ====================

TEST(DataParserTest, ToInt_Normal) {
    EXPECT_EQ(DataParser::toInt("42"), 42);
    EXPECT_EQ(DataParser::toInt("-7"), -7);
    EXPECT_EQ(DataParser::toInt("0"), 0);
}

TEST(DataParserTest, ToInt_Boundary) {
    EXPECT_FALSE(DataParser::toInt("").has_value());
    EXPECT_FALSE(DataParser::toInt("  ").has_value());
}

TEST(DataParserTest, ToInt_Invalid) {
    // 非纯数字返回 nullopt
    EXPECT_FALSE(DataParser::toInt("abc").has_value());
    EXPECT_FALSE(DataParser::toInt("12abc").has_value());
}

// ==================== toDouble ====================

TEST(DataParserTest, ToDouble_Normal) {
    EXPECT_DOUBLE_EQ(DataParser::toDouble("3.14").value(), 3.14);
    EXPECT_DOUBLE_EQ(DataParser::toDouble("-2.5").value(), -2.5);
    EXPECT_DOUBLE_EQ(DataParser::toDouble("0").value(), 0.0);
}

TEST(DataParserTest, ToDouble_Boundary) {
    EXPECT_FALSE(DataParser::toDouble("").has_value());
    EXPECT_FALSE(DataParser::toDouble("abc").has_value());
}

TEST(DataParserTest, ToDouble_Invalid) {
    EXPECT_FALSE(DataParser::toDouble("3.14abc").has_value());
}

// ==================== toBool ====================

TEST(DataParserTest, ToBool_Normal) {
    EXPECT_EQ(DataParser::toBool("true"), true);
    EXPECT_EQ(DataParser::toBool("false"), false);
    EXPECT_EQ(DataParser::toBool("1"), true);
    EXPECT_EQ(DataParser::toBool("0"), false);
    EXPECT_EQ(DataParser::toBool("yes"), true);
    EXPECT_EQ(DataParser::toBool("no"), false);
}

TEST(DataParserTest, ToBool_CaseInsensitive) {
    EXPECT_EQ(DataParser::toBool("TRUE"), true);
    EXPECT_EQ(DataParser::toBool("True"), true);
    EXPECT_EQ(DataParser::toBool("FALSE"), false);
    EXPECT_EQ(DataParser::toBool("Yes"), true);
}

TEST(DataParserTest, ToBool_Invalid) {
    EXPECT_FALSE(DataParser::toBool("maybe").has_value());
    EXPECT_FALSE(DataParser::toBool("2").has_value());
    EXPECT_FALSE(DataParser::toBool("").has_value());
}

// ==================== parseIni ====================

TEST(DataParserTest, ParseIni_Normal) {
    std::string ini = "[database]\nhost=localhost\nport=5432\n\n[server]\nport=8080\n";
    auto result = DataParser::parseIni(ini);
    EXPECT_EQ(result["database"]["host"], "localhost");
    EXPECT_EQ(result["database"]["port"], "5432");
    EXPECT_EQ(result["server"]["port"], "8080");
}

TEST(DataParserTest, ParseIni_Boundary) {
    // 空 INI
    auto empty = DataParser::parseIni("");
    EXPECT_TRUE(empty.empty());

    // 无 section 的键值对归入默认 section
    auto result = DataParser::parseIni("key=value");
    EXPECT_EQ(result[""]["key"], "value");
}

TEST(DataParserTest, ParseIni_Comments) {
    // 注释行应被跳过
    std::string ini = "[test]\n; comment\nkey=val\n# another comment\nkey2=val2\n";
    auto result = DataParser::parseIni(ini);
    EXPECT_EQ(result["test"].size(), 2u);
    EXPECT_EQ(result["test"]["key"], "val");
    EXPECT_EQ(result["test"]["key2"], "val2");
}

// ==================== parseValue ====================

TEST(DataParserTest, ParseValue_Bool) {
    EXPECT_EQ(std::get<bool>(DataParser::parseValue("true")), true);
    EXPECT_EQ(std::get<bool>(DataParser::parseValue("false")), false);
}

TEST(DataParserTest, ParseValue_Int) {
    EXPECT_EQ(std::get<int>(DataParser::parseValue("42")), 42);
    EXPECT_EQ(std::get<int>(DataParser::parseValue("-7")), -7);
}

TEST(DataParserTest, ParseValue_Double) {
    EXPECT_DOUBLE_EQ(std::get<double>(DataParser::parseValue("3.14")), 3.14);
}

TEST(DataParserTest, ParseValue_String) {
    // 带引号的字符串
    EXPECT_EQ(std::get<std::string>(DataParser::parseValue("\"hello\"")), "hello");
    EXPECT_EQ(std::get<std::string>(DataParser::parseValue("'world'")), "world");
    // 无引号 fallback
    EXPECT_EQ(std::get<std::string>(DataParser::parseValue("plaintext")), "plaintext");
}

TEST(DataParserTest, ParseValue_Null) {
    // null 和 nullptr 都解析为空字符串
    EXPECT_EQ(std::get<std::string>(DataParser::parseValue("null")), "");
    EXPECT_EQ(std::get<std::string>(DataParser::parseValue("nullptr")), "");
}

// ==================== escape / unescape ====================

TEST(DataParserTest, Escape_Normal) {
    EXPECT_EQ(DataParser::escape("a\"b"), "a\\\"b");
    EXPECT_EQ(DataParser::escape("a\\b"), "a\\\\b");
    EXPECT_EQ(DataParser::escape("a\nb"), "a\\nb");
    EXPECT_EQ(DataParser::escape("a\tb"), "a\\tb");
    EXPECT_EQ(DataParser::escape("a\rb"), "a\\rb");
}

TEST(DataParserTest, Escape_NoSpecial) {
    EXPECT_EQ(DataParser::escape("hello"), "hello");
    EXPECT_EQ(DataParser::escape(""), "");
}

TEST(DataParserTest, Unescape_Normal) {
    EXPECT_EQ(DataParser::unescape("a\\\"b"), "a\"b");
    EXPECT_EQ(DataParser::unescape("a\\\\b"), "a\\b");
    EXPECT_EQ(DataParser::unescape("a\\nb"), "a\nb");
    EXPECT_EQ(DataParser::unescape("a\\tb"), "a\tb");
    EXPECT_EQ(DataParser::unescape("a\\rb"), "a\rb");
}

TEST(DataParserTest, EscapeUnescape_RoundTrip) {
    // 转义后再反转义，应还原
    std::string original = "hello \"world\"\n\ttab\\end";
    EXPECT_EQ(DataParser::unescape(DataParser::escape(original)), original);
}

TEST(DataParserTest, EscapeUnescape_Empty) {
    EXPECT_EQ(DataParser::unescape(DataParser::escape("")), "");
}
