#include <gtest/gtest.h>
#include "calculator.h"

// Tests for Calculator static methods
TEST(CalculatorTest, Add) {
    EXPECT_DOUBLE_EQ(Calculator::add(2.0, 3.0), 5.0);
    EXPECT_DOUBLE_EQ(Calculator::add(0.0, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(Calculator::add(-2.0, 5.0), 3.0);
}

TEST(CalculatorTest, Subtract) {
    EXPECT_DOUBLE_EQ(Calculator::subtract(5.0, 3.0), 2.0);
    EXPECT_DOUBLE_EQ(Calculator::subtract(0.0, 5.0), -5.0);
    EXPECT_DOUBLE_EQ(Calculator::subtract(-2.0, -3.0), 1.0);
}

TEST(CalculatorTest, Multiply) {
    EXPECT_DOUBLE_EQ(Calculator::multiply(3.0, 4.0), 12.0);
    EXPECT_DOUBLE_EQ(Calculator::multiply(0.0, 5.0), 0.0);
    EXPECT_DOUBLE_EQ(Calculator::multiply(-2.0, 3.0), -6.0);
}

TEST(CalculatorTest, Divide) {
    EXPECT_DOUBLE_EQ(Calculator::divide(6.0, 3.0), 2.0);
    EXPECT_THROW(Calculator::divide(1.0, 0.0), std::runtime_error);
}

TEST(CalculatorTest, Calculate) {
    EXPECT_DOUBLE_EQ(Calculator::calculate(2.0, '+', 3.0), 5.0);
    EXPECT_DOUBLE_EQ(Calculator::calculate(5.0, '-', 2.0), 3.0);
    EXPECT_DOUBLE_EQ(Calculator::calculate(4.0, '*', 3.0), 12.0);
    EXPECT_DOUBLE_EQ(Calculator::calculate(8.0, '/', 2.0), 4.0);
    EXPECT_THROW(Calculator::calculate(2.0, '%', 5.0), std::invalid_argument);
    EXPECT_THROW(Calculator::calculate(1.0, '/', 0.0), std::runtime_error);
}

TEST(CalculatorTest, Factorial) {
    EXPECT_EQ(Calculator::factorial(0), 1);
    EXPECT_EQ(Calculator::factorial(5), 120);
    EXPECT_EQ(Calculator::factorial(12), 479001600);
    EXPECT_THROW(Calculator::factorial(-1), std::invalid_argument);
}

TEST(CalculatorTest, Power) {
    EXPECT_DOUBLE_EQ(Calculator::power(2.0, 3), 8.0);
    EXPECT_DOUBLE_EQ(Calculator::power(5.0, 0), 1.0);
    EXPECT_DOUBLE_EQ(Calculator::power(2.0, -2), 0.25);
}

TEST(CalculatorTest, IsPrime) {
    EXPECT_TRUE(Calculator::isPrime(2));
    EXPECT_TRUE(Calculator::isPrime(17));
    EXPECT_FALSE(Calculator::isPrime(1));
    EXPECT_FALSE(Calculator::isPrime(18));
    EXPECT_FALSE(Calculator::isPrime(-7));
}

TEST(CalculatorTest, Gcd) {
    EXPECT_EQ(Calculator::gcd(48, 18), 6);
    EXPECT_EQ(Calculator::gcd(0, 5), 5);
    EXPECT_EQ(Calculator::gcd(0, 0), 0);
}
