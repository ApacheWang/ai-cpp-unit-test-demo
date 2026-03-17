#include "data_parser.h"
#include <sstream>
#include <cctype>
#include <algorithm>
#include <charconv>

std::pair<std::string, std::string> DataParser::parseKeyValue(const std::string& input) {
    auto pos = input.find('=');
    if (pos == std::string::npos || pos == 0) {
        throw std::invalid_argument("Invalid key-value format: " + input);
    }
    std::string key = input.substr(0, pos);
    std::string value = (pos + 1 < input.size()) ? input.substr(pos + 1) : "";
    return {trim(key), trim(value)};
}

std::vector<int> DataParser::parseIntList(const std::string& input) {
    std::vector<int> result;
    std::istringstream ss(input);
    std::string token;
    while (std::getline(ss, token, ',')) {
        auto trimmed = trim(token);
        if (trimmed.empty()) continue;
        try {
            result.push_back(std::stoi(trimmed));
        } catch (...) {
            // 跳过无效项
        }
    }
    return result;
}

std::optional<int> DataParser::toInt(const std::string& str) {
    if (str.empty()) return std::nullopt;
    try {
        size_t pos;
        int val = std::stoi(str, &pos);
        if (pos == str.size()) return val;
        return std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<double> DataParser::toDouble(const std::string& str) {
    if (str.empty()) return std::nullopt;
    try {
        size_t pos;
        double val = std::stod(str, &pos);
        if (pos == str.size()) return val;
        return std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<bool> DataParser::toBool(const std::string& str) {
    auto lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower == "true" || lower == "1" || lower == "yes") return true;
    if (lower == "false" || lower == "0" || lower == "no") return false;
    return std::nullopt;
}

std::unordered_map<std::string, std::unordered_map<std::string, std::string>>
DataParser::parseIni(const std::string& content) {
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> result;
    std::string currentSection;

    std::istringstream ss(content);
    std::string line;
    while (std::getline(ss, line)) {
        auto trimmed = trim(line);
        // 跳过空行和注释
        if (trimmed.empty() || trimmed[0] == ';' || trimmed[0] == '#') continue;

        // Section header
        if (trimmed[0] == '[' && trimmed.back() == ']') {
            currentSection = trim(trimmed.substr(1, trimmed.size() - 2));
            continue;
        }

        // Key-value pair
        auto pos = trimmed.find('=');
        if (pos != std::string::npos) {
            std::string key = trim(trimmed.substr(0, pos));
            std::string value = trim(trimmed.substr(pos + 1));
            result[currentSection][key] = value;
        }
    }
    return result;
}

DataParser::Value DataParser::parseValue(const std::string& input) {
    auto trimmed = trim(input);
    if (trimmed.empty()) return std::string("");

    // Bool
    if (trimmed == "true") return true;
    if (trimmed == "false") return false;

    // Null
    if (trimmed == "null" || trimmed == "nullptr") return std::string("");

    // String (quoted)
    if ((trimmed.front() == '"' && trimmed.back() == '"') ||
        (trimmed.front() == '\'' && trimmed.back() == '\'')) {
        return trimmed.substr(1, trimmed.size() - 2);
    }

    // Try int
    auto intVal = toInt(trimmed);
    if (intVal.has_value()) return intVal.value();

    // Try double
    auto dblVal = toDouble(trimmed);
    if (dblVal.has_value()) return dblVal.value();

    // Fallback: string
    return trimmed;
}

std::string DataParser::escape(const std::string& str) {
    std::string result;
    for (char c : str) {
        switch (c) {
            case '"':  result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n";  break;
            case '\t': result += "\\t";  break;
            case '\r': result += "\\r";  break;
            default:   result += c;      break;
        }
    }
    return result;
}

std::string DataParser::unescape(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '\\' && i + 1 < str.size()) {
            switch (str[i + 1]) {
                case '"':  result += '"';  ++i; break;
                case '\\': result += '\\'; ++i; break;
                case 'n':  result += '\n'; ++i; break;
                case 't':  result += '\t'; ++i; break;
                case 'r':  result += '\r'; ++i; break;
                default:   result += str[i];     break;
            }
        } else {
            result += str[i];
        }
    }
    return result;
}

std::string DataParser::trim(const std::string& str) {
    auto start = std::find_if(str.begin(), str.end(),
                              [](unsigned char c) { return !std::isspace(c); });
    auto end = std::find_if(str.rbegin(), str.rend(),
                            [](unsigned char c) { return !std::isspace(c); });
    return (start < end.base()) ? std::string(start, end.base()) : "";
}
