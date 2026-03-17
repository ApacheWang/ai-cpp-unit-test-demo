/**
 * test_calculator.cpp — AI 生成的 Calculator 单元测试
 *
 * 演示说明：
 *   本文件由 OpenCode AI 根据源码自动生成，覆盖以下场景：
 *   - 正常运算路径
 *   - 边界条件（零、负数、极大值）
 *   - 异常路径（除零、非法运算符、负数阶乘）
 *   - 数学属性验证（交换律、结合律等）
 *
 * 在演示时，可以删除本文件，然后在 OpenCode 中输入 prompt 重新生成：
 *   "为 include/calculator.h 和 src/calculator.cpp 生成完整的 Google Test 单元测试，
 *    要求覆盖所有公开方法、边界条件和异常情况，输出到 tests/test_calculator.cpp。
 *    使用 TEST 和 TEST_F 组织用例，每个方法至少 3 个测试。"
 */

#include <gtest/gtest.h>
#include "calculator.h"
#include <limits>
#include <cmath>

using namespace std;

// ============================================================
// 1. 基础四则运算
// ============================================================

TEST(CalculatorTest, AddPositiveNumbers) {
    EXPECT_DOUBLE_EQ(Calculator::add(2.0, 3.0), 5.0);
    EXPECT_DOUBLE_EQ(Calculator::add(0.0, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(Calculator::add(-1.0, 1.0), 0.0);
}

TEST(CalculatorTest, AddEdgeCases) {
    EXPECT_DOUBLE_EQ(Calculator::add(numeric_limits<double>::max(), 0.0),
                     numeric_limits<double>::max());
    EXPECT_DOUBLE_EQ(Calculator::add(numeric_limits<double>::lowest(), 0.0),
                     numeric_limits<double>::lowest());
    EXPECT_DOUBLE_EQ(Calculator::add(1e308, -1e308), 0.0); // 极大值抵消
}

TEST(CalculatorTest, SubtractBasic) {
    EXPECT_DOUBLE_EQ(Calculator::subtract(5.0, 3.0), 2.0);
    EXPECT_DOUBLE_EQ(Calculator::subtract(0.0, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(Calculator::subtract(3.0, 5.0), -2.0);
}

TEST(CalculatorTest, MultiplyBasic) {
    EXPECT_DOUBLE_EQ(Calculator::multiply(2.0, 3.0), 6.0);
    EXPECT_DOUBLE_EQ(Calculator::multiply(-2.0, 3.0), -6.0);
    EXPECT_DOUBLE_EQ(Calculator::multiply(0.0, 100.0), 0.0);
}

TEST(CalculatorTest, MultiplyEdgeCases) {
    EXPECT_DOUBLE_EQ(Calculator::multiply(1.0, numeric_limits<double>::max()),
                     numeric_limits<double>::max());
    EXPECT_DOUBLE_EQ(Calculator::multiply(-1.0, numeric_limits<double>::max()),
                     -numeric_limits<double>::max());
    EXPECT_DOUBLE_EQ(Calculator::multiply(0.001, 0.001), 1e-6);
}

TEST(CalculatorTest, DivideBasic) {
    EXPECT_DOUBLE_EQ(Calculator::divide(6.0, 2.0), 3.0);
    EXPECT_DOUBLE_EQ(Calculator::divide(5.0, 2.0), 2.5);
    EXPECT_DOUBLE_EQ(Calculator::divide(0.0, 5.0), 0.0);
}

TEST(CalculatorTest, DivideByZeroThrows) {
    EXPECT_THROW(Calculator::divide(1.0, 0.0), std::runtime_error);
    EXPECT_THROW(Calculator::divide(-1.0, 0.0), std::runtime_error);
    EXPECT_THROW(Calculator::divide(0.0, 0.0), std::runtime_error);
}

// ============================================================
// 2. calculate() 统一接口
// ============================================================

TEST(CalculatorTest, CalculateBasic) {
    EXPECT_DOUBLE_EQ(Calculator::calculate(10, '+', 5), 15.0);
    EXPECT_DOUBLE_EQ(Calculator::calculate(10, '-', 5), 5.0);
    EXPECT_DOUBLE_EQ(Calculator::calculate(10, '*', 5), 50.0);
    EXPECT_DOUBLE_EQ(Calculator::calculate(10, '/', 5), 2.0);
}

TEST(CalculatorTest, CalculateInvalidOperator) {
    EXPECT_THROW(Calculator::calculate(1, '%', 2), std::invalid_argument);
    EXPECT_THROW(Calculator::calculate(1, '^', 2), std::invalid_argument);
    EXPECT_THROW(Calculator::calculate(1, 'x', 2), std::invalid_argument);
}

// ============================================================
// 3. 阶乘
// ============================================================

TEST(CalculatorTest, FactorialBasic) {
    EXPECT_EQ(Calculator::factorial(0), 1ULL);
    EXPECT_EQ(Calculator::factorial(1), 1ULL);
    EXPECT_EQ(Calculator::factorial(5), 120ULL);
    EXPECT_EQ(Calculator::factorial(10), 3628800ULL);
}

TEST(CalculatorTest, FactorialLarge) {
    EXPECT_EQ(Calculator::factorial(15), 1307674368000ULL);
    EXPECT_EQ(Calculator::factorial(20), 2432902008176640000ULL);
}

TEST(CalculatorTest, FactorialNegativeThrows) {
    EXPECT_THROW(Calculator::factorial(-1), std::invalid_argument);
    EXPECT_THROW(Calculator::factorial(-100), std::invalid_argument);
}

// ============================================================
// 4. 幂运算
// ============================================================

TEST(CalculatorTest, PowerBasic) {
    EXPECT_DOUBLE_EQ(Calculator::power(2, 10), 1024.0);
    EXPECT_DOUBLE_EQ(Calculator::power(3, 0), 1.0);
    EXPECT_DOUBLE_EQ(Calculator::power(5, 1), 5.0);
    EXPECT_DOUBLE_EQ(Calculator::power(2, -1), 0.5);
}

TEST(CalculatorTest, PowerZeroBase) {
    EXPECT_DOUBLE_EQ(Calculator::power(0, 5), 0.0);
    EXPECT_DOUBLE_EQ(Calculator::power(0, 0), 1.0); // 0^0 = 1 by convention
}

TEST(CalculatorTest, PowerNegativeBase) {
    EXPECT_DOUBLE_EQ(Calculator::power(-2, 3), -8.0);
    EXPECT_DOUBLE_EQ(Calculator::power(-2, 2), 4.0);
    EXPECT_DOUBLE_EQ(Calculator::power(-1, 100), 1.0);
}

// ============================================================
// 5. 素数判定
// ============================================================

TEST(CalculatorTest, IsPrimeBasic) {
    EXPECT_FALSE(Calculator::isPrime(0));
    EXPECT_FALSE(Calculator::isPrime(1));
    EXPECT_TRUE(Calculator::isPrime(2));
    EXPECT_TRUE(Calculator::isPrime(3));
    EXPECT_FALSE(Calculator::isPrime(4));
    EXPECT_TRUE(Calculator::isPrime(5));
    EXPECT_TRUE(Calculator::isPrime(97));
    EXPECT_FALSE(Calculator::isPrime(100));
}

TEST(CalculatorTest, IsPrimeNegative) {
    EXPECT_FALSE(Calculator::isPrime(-1));
    EXPECT_FALSE(Calculator::isPrime(-7));
}

TEST(CalculatorTest, IsPrimeLarge) {
    EXPECT_TRUE(Calculator::isPrime(7919));   // known prime
    EXPECT_FALSE(Calculator::isPrime(7920));  // 7920 = 7919 + 1
}

// ============================================================
// 6. 最大公约数
// ============================================================

TEST(CalculatorTest, GcdBasic) {
    EXPECT_EQ(Calculator::gcd(12, 8), 4);
    EXPECT_EQ(Calculator::gcd(7, 3), 1);
    EXPECT_EQ(Calculator::gcd(100, 100), 100);
    EXPECT_EQ(Calculator::gcd(0, 5), 5);
}

TEST(CalculatorTest, GcdCommutative) {
    EXPECT_EQ(Calculator::gcd(48, 18), Calculator::gcd(18, 48));
    EXPECT_EQ(Calculator::gcd(17, 13), Calculator::gcd(13, 17));
}

TEST(CalculatorTest, GcdNegative) {
    EXPECT_EQ(Calculator::gcd(-12, 8), 4);
    EXPECT_EQ(Calculator::gcd(12, -8), 4);
    EXPECT_EQ(Calculator::gcd(-12, -8), 4);
}

// ============================================================
// 7. 数学属性验证（AI 特别擅长生成的用例）
// ============================================================

TEST(CalculatorTest, AdditiveIdentity) {
    // a + 0 = a
    for (double v : {-100.0, -1.0, 0.0, 1.0, 42.0, 1e100}) {
        EXPECT_DOUBLE_EQ(Calculator::add(v, 0.0), v);
        EXPECT_DOUBLE_EQ(Calculator::add(0.0, v), v);
    }
}

TEST(CalculatorTest, MultiplicativeIdentity) {
    // a * 1 = a
    for (double v : {-100.0, -1.0, 0.0, 1.0, 42.0}) {
        EXPECT_DOUBLE_EQ(Calculator::multiply(v, 1.0), v);
        EXPECT_DOUBLE_EQ(Calculator::multiply(1.0, v), v);
    }
}

TEST(CalculatorTest, DistributiveProperty) {
    // a * (b + c) = a*b + a*c
    double a = 3.0, b = 4.0, c = 5.0;
    EXPECT_DOUBLE_EQ(
        Calculator::multiply(a, Calculator::add(b, c)),
        Calculator::add(Calculator::multiply(a, b), Calculator::multiply(a, c))
    );
}

TEST(CalculatorTest, PowerOfOne) {
    // a^1 = a
    EXPECT_DOUBLE_EQ(Calculator::power(42.0, 1), 42.0);
    EXPECT_DOUBLE_EQ(Calculator::power(-7.0, 1), -7.0);
    EXPECT_DOUBLE_EQ(Calculator::power(0.0, 1), 0.0);
}
