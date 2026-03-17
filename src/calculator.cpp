#include "calculator.h"
#include <cmath>
#include <algorithm>

double Calculator::calculate(double a, char op, double b) {
    switch (op) {
        case '+': return add(a, b);
        case '-': return subtract(a, b);
        case '*': return multiply(a, b);
        case '/': return divide(a, b);
        default:
            throw std::invalid_argument("Unknown operator: " + std::string(1, op));
    }
}

double Calculator::add(double a, double b) {
    return a + b;
}

double Calculator::subtract(double a, double b) {
    return a - b;
}

double Calculator::multiply(double a, double b) {
    return a * b;
}

double Calculator::divide(double a, double b) {
    if (b == 0.0) {
        throw std::runtime_error("Division by zero");
    }
    return a / b;
}

uint64_t Calculator::factorial(int n) {
    if (n < 0) {
        throw std::invalid_argument("Factorial is not defined for negative numbers");
    }
    uint64_t result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= static_cast<uint64_t>(i);
    }
    return result;
}

double Calculator::power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    if (exponent < 0) {
        return 1.0 / power(base, -exponent);
    }
    double result = 1.0;
    double current = base;
    int exp = exponent;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result *= current;
        }
        current *= current;
        exp /= 2;
    }
    return result;
}

bool Calculator::isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

int Calculator::gcd(int a, int b) {
    a = std::abs(a);
    b = std::abs(b);
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}
