#pragma once

#include <string>
#include <vector>
#include <algorithm>

class StringUtils {
public:
    /// 转大写
    static std::string toUpper(const std::string& str);

    /// 转小写
    static std::string toLower(const std::string& str);

    /// 去除首尾空白
    static std::string trim(const std::string& str);

    /// 去除左侧空白
    static std::string trimLeft(const std::string& str);

    /// 去除右侧空白
    static std::string trimRight(const std::string& str);

    /// 字符串分割
    /// @param str 原始字符串
    /// @param delimiter 分隔符
    /// @param keepEmpty 是否保留空字符串
    static std::vector<std::string> split(const std::string& str,
                                           const std::string& delimiter,
                                           bool keepEmpty = false);

    /// 字符串替换（全部）
    static std::string replace(const std::string& str,
                                const std::string& from,
                                const std::string& to);

    /// 判断是否以指定前缀开头
    static bool startsWith(const std::string& str, const std::string& prefix);

    /// 判断是否以指定后缀结尾
    static bool endsWith(const std::string& str, const std::string& suffix);

    /// 反转字符串
    static std::string reverse(const std::string& str);

    /// 重复字符串
    static std::string repeat(const std::string& str, int count);

    /// 驼峰转下划线 (camelCase -> camel_case)
    static std::string camelToSnake(const std::string& str);
};
