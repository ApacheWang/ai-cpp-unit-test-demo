#pragma once

#include <string>
#include <stdexcept>
#include <cstdint>

class Calculator {
public:
    /// 执行四则运算
    /// @param a 左操作数
    /// @param op 运算符: +, -, *, /
    /// @param b 右操作数
    /// @return 运算结果
    /// @throws std::invalid_argument 未知运算符
    /// @throws std::runtime_error 除零错误
    static double calculate(double a, char op, double b);

    /// 加法
    static double add(double a, double b);

    /// 减法
    static double subtract(double a, double b);

    /// 乘法
    static double multiply(double a, double b);

    /// 除法
    /// @throws std::runtime_error 除零
    static double divide(double a, double b);

    /// 阶乘（仅支持非负整数）
    /// @throws std::invalid_argument 负数或非整数
    static uint64_t factorial(int n);

    /// 幂运算
    static double power(double base, int exponent);

    /// 判断素数
    static bool isPrime(int n);

    /// 最大公约数
    static int gcd(int a, int b);
};
