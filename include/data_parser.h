#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <variant>

class DataParser {
public:
    /// 解析结果类型
    using Value = std::variant<int, double, std::string, bool>;

    /// 解析键值对字符串（格式: "key=value"）
    /// @throws std::invalid_argument 格式错误
    static std::pair<std::string, std::string> parseKeyValue(const std::string& input);

    /// 解析逗号分隔的整数列表（格式: "1,2,3"）
    static std::vector<int> parseIntList(const std::string& input);

    /// 安全类型转换：字符串 -> int
    static std::optional<int> toInt(const std::string& str);

    /// 安全类型转换：字符串 -> double
    static std::optional<double> toDouble(const std::string& str);

    /// 安全类型转换：字符串 -> bool
    static std::optional<bool> toBool(const std::string& str);

    /// 简易 INI 解析（格式: "[section]\nkey=value"）
    /// @return section -> {key -> value}
    static std::unordered_map<std::string, std::unordered_map<std::string, std::string>>
    parseIni(const std::string& content);

    /// JSON-like 值解析（仅支持基础类型：数字、字符串、布尔值、null）
    static Value parseValue(const std::string& input);

    /// 转义字符串中的特殊字符（用于输出）
    static std::string escape(const std::string& str);

    /// 反转义
    static std::string unescape(const std::string& str);

private:
    static std::string trim(const std::string& str);
};
