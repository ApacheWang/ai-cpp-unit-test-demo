#include "string_utils.h"
#include <cctype>

std::string StringUtils::toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

std::string StringUtils::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string StringUtils::trim(const std::string& str) {
    return trimRight(trimLeft(str));
}

std::string StringUtils::trimLeft(const std::string& str) {
    auto it = std::find_if(str.begin(), str.end(),
                           [](unsigned char c) { return !std::isspace(c); });
    return std::string(it, str.end());
}

std::string StringUtils::trimRight(const std::string& str) {
    auto it = std::find_if(str.rbegin(), str.rend(),
                           [](unsigned char c) { return !std::isspace(c); });
    return std::string(str.begin(), it.base());
}

std::vector<std::string> StringUtils::split(const std::string& str,
                                              const std::string& delimiter,
                                              bool keepEmpty) {
    std::vector<std::string> result;
    if (delimiter.empty()) {
        if (!str.empty() || keepEmpty) {
            result.push_back(str);
        }
        return result;
    }

    size_t start = 0;
    size_t end = str.find(delimiter);
    while (end != std::string::npos) {
        std::string token = str.substr(start, end - start);
        if (keepEmpty || !token.empty()) {
            result.push_back(token);
        }
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    std::string token = str.substr(start);
    if (keepEmpty || !token.empty()) {
        result.push_back(token);
    }
    return result;
}

std::string StringUtils::replace(const std::string& str,
                                   const std::string& from,
                                   const std::string& to) {
    if (from.empty()) return str;
    std::string result = str;
    size_t pos = 0;
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.length(), to);
        pos += to.length();
    }
    return result;
}

bool StringUtils::startsWith(const std::string& str, const std::string& prefix) {
    if (prefix.size() > str.size()) return false;
    return str.compare(0, prefix.size(), prefix) == 0;
}

bool StringUtils::endsWith(const std::string& str, const std::string& suffix) {
    if (suffix.size() > str.size()) return false;
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string StringUtils::reverse(const std::string& str) {
    std::string result = str;
    std::reverse(result.begin(), result.end());
    return result;
}

std::string StringUtils::repeat(const std::string& str, int count) {
    if (count <= 0) return "";
    std::string result;
    result.reserve(str.size() * count);
    for (int i = 0; i < count; ++i) {
        result += str;
    }
    return result;
}

std::string StringUtils::camelToSnake(const std::string& str) {
    if (str.empty()) return str;
    std::string result;
    result += std::tolower(str[0]);
    for (size_t i = 1; i < str.size(); ++i) {
        if (std::isupper(str[i])) {
            result += '_';
            result += std::tolower(str[i]);
        } else {
            result += str[i];
        }
    }
    return result;
}
