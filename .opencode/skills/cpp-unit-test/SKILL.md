---
name: cpp-unit-test
description: "Systematic C++ unit test generation with Google Test. Covers header analysis, interface categorization, coverage matrix generation, assertion selection, fixture design, parameterized tests, type-parametrized tests, mock/stub patterns, CMake integration, and quality audit. Use when generating, extending, reviewing, or fixing GTest unit tests for C++ projects."
license: MIT
compatibility: opencode
metadata:
  language: cpp17
  framework: google-test
  build-system: cmake
  workflow: test-generation
---

# C++ Unit Test Generation Skill

A systematic approach to generating high-quality Google Test unit tests for C++ projects.

## When to Use

- Generate unit tests for a C++ module
- Extend existing tests with missing coverage
- Review and improve test quality
- Fix compilation or logic errors in tests
- Create test infrastructure (fixtures, mocks, CMake config)

## Phase 0 — Pre-Flight Checks

Before writing any test code, the agent MUST:

1. **Read the header file** of the module under test to understand the exact API signatures
2. **Read the implementation file** to understand behavior, error handling, and edge cases
3. **Check for existing tests** in `tests/` to avoid duplication
4. **Identify the test framework** — this skill targets Google Test (GTest)
5. **Check project build system** — this skill targets CMake

### Critical Rule

> **NEVER guess API signatures.** Always read the actual header file. Common mistakes:
> - Assuming functions return bool when they return std::optional<bool>
> - Assuming functions throw exceptions when they return error codes or nullopt
> - Missing default parameters, overloaded signatures, or template methods
> - Wrong namespace, wrong class name, wrong include path

## Phase 1 — Interface Analysis

After reading the header, categorize every public method into types:

| Category | Description | Example |
|----------|-------------|---------|
| **Pure Function** | No side effects, output determined solely by input | `int add(int, int)` |
| **Stateful Method** | Operates on object state | `void push(const T&)` |
| **Query Method** | Returns info about object state | `size_t size() const` |
| **Factory/Constructor** | Creates objects | `static Widget create()` |
| **Mutator** | Changes object state, returns void or new state | `void clear()` |
| **Exception Path** | Documented to throw on certain inputs | `T& at(size_t)` throws `std::out_of_range` |
| **Error Code Path** | Returns error indicator instead of throwing | `std::optional<T> parse(str)` |
| **Template Method** | Operates on type parameter | `template<typename T> T convert(const std::string&)` |

For each method, also note:
- **Const correctness** — is it `const`?
- **Reference/pointer semantics** — by value, by `const&`, by `&&`, by pointer?
- **Move semantics** — does it support move? steal resources?
- **Exception specification** — `noexcept`? documented throws?
- **Preconditions** — what inputs are valid?
- **Postconditions** — what guarantees does it provide?

## Phase 2 — Coverage Matrix

For each method, generate a coverage matrix with these rows:

### 2.1 Required Rows (Every Method)

| Row | Description |
|-----|-------------|
| **Happy Path** | Typical valid input → expected output |
| **Empty/Zero Input** | Empty string, zero, empty container, null pointer |
| **Single Element** | Minimum non-trivial input |
| **Boundary Max** | Largest typical value (INT_MAX, large string, full container) |
| **Negative/Invalid Input** | Negative numbers, malformed strings, out-of-range indices |

### 2.2 Category-Specific Rows

**For numeric operations:**
- Overflow boundaries (INT_MAX + 1, INT_MIN - 1)
- Floating-point: NaN, Inf, -Inf, -0.0, denormalized
- Precision: values that differ by ULP

**For string operations:**
- Empty string, single char, long string (>4096 chars)
- Whitespace-only strings, strings with null bytes
- Unicode / multi-byte characters (if applicable)
- Strings with special chars: `\0`, `\n`, `\t`, `\r`, `'`, `"`, `\`

**For container operations:**
- Empty container, single element, full capacity
- Duplicate elements, sorted vs unsorted input
- Mixed types (if heterogenous container)

**For parsing/validation:**
- Valid inputs of all supported formats
- Partially valid, malformed, completely invalid
- Boundary between valid and invalid
- Extra whitespace, trailing delimiters, missing delimiters

**For error code / optional return paths:**
- Input that returns `std::nullopt` or error code
- Input that returns a valid value (verify both branches)
- Boundary between success and failure

**For exception-throwing paths:**
- Input that triggers documented exception
- Input near the boundary (just valid, just invalid)
- Multiple exception types (if applicable)

### 2.3 Mathematical / Logical Properties (If Applicable)

- **Commutativity**: `f(a,b) == f(b,a)`
- **Associativity**: `f(f(a,b),c) == f(a,f(b,c))`
- **Identity**: `f(x, identity) == x`
- **Inverse**: `f(f(x)) == x` (round-trip)
- **Idempotence**: `f(f(x)) == f(x)`
- **Monotonicity**: `a < b` implies `f(a) < f(b)`
- **Distributivity**: `f(a, g(b,c)) == g(f(a,b), f(a,c))`

## Phase 3 — Test Code Generation Rules

### 3.1 File Structure

```
tests/
├── CMakeLists.txt
├── test_<module_name>.cpp
└── helpers/
    ├── test_helpers.h      # Shared utilities, custom matchers
    └── mocks/              # Mock implementations (if needed)
```

### 3.2 Test Naming Convention

```cpp
// Format: TEST(SuiteName, ScenarioDescription)
TEST(CalculatorTest, Add_TwoPositiveNumbers)
TEST(CalculatorTest, Add_PositiveAndNegative)
TEST(CalculatorTest, Divide_ByZero_ThrowsRuntimeError)

// Fixture: TEST_F(FixtureName, ScenarioDescription)
TEST_F(VectorTest, PushBack_IncreasesSize)

// Parameterized: TEST_P(FixtureName, ScenarioDescription)
TEST_P(StringTrimTest, RemovesLeadingWhitespace)
```

**Rules:**
- Suite name = `ClassName + Test` (e.g., `CalculatorTest`, `DataParserTest`)
- Scenario description = `MethodName + _ + Condition` or `Condition_ExpectedBehavior`
- Use PascalCase for words in scenario
- Describe *behavior*, not *implementation*

### 3.3 Assertion Selection Guide

| Situation | Assertion | Example |
|-----------|-----------|---------|
| Exact equality | `EXPECT_EQ` / `ASSERT_EQ` | `EXPECT_EQ(result, 42)` |
| Inequality | `EXPECT_NE` / `ASSERT_NE` | `EXPECT_NE(ptr, nullptr)` |
| Boolean condition | `EXPECT_TRUE` / `EXPECT_FALSE` | `EXPECT_TRUE(isValid)` |
| Float comparison | `EXPECT_DOUBLE_EQ` | `EXPECT_DOUBLE_EQ(result, 3.14)` |
| Float with tolerance | `EXPECT_NEAR(val, expected, abs_error)` | `EXPECT_NEAR(pi, 3.14159, 0.00001)` |
| Exception expected | `EXPECT_THROW(stmt, type)` | `EXPECT_THROW(divide(1,0), std::runtime_error)` |
| Any exception | `EXPECT_ANY_THROW(stmt)` | `EXPECT_ANY_THROW(parse("!!!"))` |
| No exception | `EXPECT_NO_THROW(stmt)` | `EXPECT_NO_THROW(validator.check()) |
| String matching | `EXPECT_THAT(str, StartsWith(...))` | requires `#include <gmock/gmock.h>` |
| Container content | `EXPECT_THAT(vec, ElementsAre(...))` | requires `#include <gmock/gmock.h>` |

**Rules:**
- Prefer `EXPECT_*` (non-fatal) over `ASSERT_*` (fatal) unless the rest of the test is meaningless after failure
- Use `ASSERT_*` for setup/preconditions: `ASSERT_NE(ptr, nullptr)` before dereferencing
- For `std::optional` / `std::variant` returns: `ASSERT_TRUE(opt.has_value())` then `EXPECT_EQ(*opt, val)`
- For `std::pair` returns: use structured bindings `auto [key, val] = func()`
- Never use `EXPECT_EQ` on floating-point without `DOUBLE_EQ` or `NEAR`
- Never use `EXPECT_EQ` on `char*` (C strings) — use `EXPECT_STREQ` or convert to `std::string`

### 3.4 Fixture Design

Use `TEST_F` when multiple tests share:
- Common setup/teardown logic
- The same object state
- Expensive resource allocation

```cpp
class DataParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Runs before each test
    }

    void TearDown() override {
        // Runs after each test (even on failure)
    }

    // Shared members accessible in tests
    DataParser parser;
};
```

### 3.5 Parameterized Tests

Use when the same logic applies to a set of input/output pairs:

```cpp
// Step 1: Define fixture
struct ParseIntParams {
    std::string input;
    std::optional<int> expected;
};

class ParseIntTest : public ::testing::TestWithParam<ParseIntParams> {};

// Step 2: Write test once
TEST_P(ParseIntTest, VariousInputs) {
    auto [input, expected] = GetParam();
    EXPECT_EQ(DataParser::toInt(input), expected);
}

// Step 3: Register parameter set
INSTANTIATE_TEST_SUITE_P(
    DataParser,
    ParseIntTest,
    ::testing::Values(
        ParseIntParams{"42", 42},
        ParseIntParams{"-7", -7},
        ParseIntParams{"0", 0},
        ParseIntParams{"", std::nullopt},
        ParseIntParams{"abc", std::nullopt},
        ParseIntParams{"12abc", std::nullopt}
    )
);
```

### 3.6 Type-Parameterized Tests

For template methods that need to work across multiple types:

```cpp
template <typename T>
class NumericConversionTest : public ::testing::Test {};

using TestTypes = ::testing::Types<int, long, float, double>;
TYPED_TEST_SUITE(NumericConversionTest, TestTypes);

TYPED_TEST(NumericConversionTest, ParsesValidNumber) {
    // Use TypeParam inside the test
    // ...
}
```

### 3.7 gMock Matchers (Advanced)

Include `<gmock/gmock.h>` for powerful assertions:

```cpp
#include <gmock/gmock.h>

using ::testing::ElementsAre;
using ::testing::ElementsAreArray;
using ::testing::UnorderedElementsAre;
using ::testing::Contains;
using ::testing::StartsWith;
using ::testing::EndsWith;
using ::testing::HasSubstr;
using ::testing::MatchesRegex;
using ::testing::IsEmpty;
using ::testing::SizeIs;
using ::testing::Ge;
using ::testing::Le;
using ::testing::AllOf;
using ::testing::AnyOf;
using ::testing::Not;
using ::testing::Pair;

// Container assertions
EXPECT_THAT(result, ElementsAre(1, 2, 3));
EXPECT_THAT(result, UnorderedElementsAre(3, 1, 2));
EXPECT_THAT(result, SizeIs(3));
EXPECT_THAT(result, Contains(2));
EXPECT_THAT(map, Contains(Pair("key", "value")));

// String assertions
EXPECT_THAT(str, StartsWith("prefix"));
EXPECT_THAT(str, HasSubstr("middle"));
EXPECT_THAT(str, MatchesRegex(R"(\d{3}-\d{4})"));
```

## Phase 4 — CMake Integration

### 4.1 Test CMakeLists Template

```cmake
# tests/CMakeLists.txt

# --- Google Test (FetchContent if not installed) ---
find_package(GTest QUIET)
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# --- Enable testing ---
enable_testing()

# --- Helper library for shared test utilities ---
# add_library(test_helpers helpers/test_helpers.h)
# target_link_libraries(test_helpers PRIVATE GTest::gtest)

# --- Test targets ---
# Per-module pattern:
add_executable(test_<module> test_<module>.cpp)
target_link_libraries(test_<module>
    PRIVATE
        <your_lib>
        GTest::gtest_main
)
target_include_directories(test_<module> PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME <Module>Test COMMAND test_<module>)

# Enable C++17
target_compile_features(test_<module> PUBLIC cxx_std_17)
```

### 4.2 Running Tests

```bash
# Configure
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Build
cmake --build build

# Run all tests
cd build && ctest --output-on-failure

# Run specific test
./build/tests/test_calculator --gtest_filter="CalculatorTest.Add*"

# Run with verbose output
./build/tests/test_calculator --gtest_print_time=1
```

## Phase 5 — Quality Audit Checklist

After generating tests, self-audit against this checklist:

### Completeness
- [ ] Every public method has at least 1 test
- [ ] Every public method has happy path + error/boundary tests
- [ ] Return types are tested correctly (not assuming wrong types)
- [ ] All overloaded signatures are tested
- [ ] Template instantiations are tested for relevant types

### Correctness
- [ ] API signatures match the actual header (not guessed)
- [ ] `#include` paths are correct
- [ ] Expected values are correct (verify against implementation logic)
- [ ] Exception types match actual throws (not invented)
- [ ] No test depends on another test's execution order

### Assertion Quality
- [ ] No floating-point compared with `EXPECT_EQ` (use `DOUBLE_EQ` or `NEAR`)
- [ ] No `EXPECT_TRUE(result == expected)` (use `EXPECT_EQ` for better error messages)
- [ ] `ASSERT_*` used only for preconditions, `EXPECT_*` for everything else
- [ ] Error messages added via `<<` for ambiguous assertions
- [ ] `EXPECT_NO_THROW` used to verify non-throwing paths

### Design
- [ ] Tests are independent (no shared mutable state between TEST macros)
- [ ] Fixtures used when setup/teardown is shared
- [ ] Parameterized tests used for repetitive input/output patterns
- [ ] Test names describe behavior, not implementation
- [ ] Comments explain *why*, not *what* (code should be self-explanatory for *what*)

### Anti-Patterns to Avoid
- ❌ **Testing private methods** — test through public interface
- ❌ **Fragile assertions** — don't assert on exact error message strings, assert on exception type
- ❌ **Over-specified tests** — don't assert internal state that could change
- ❌ **Testing implementation details** — assert observable behavior
- ❌ **Giant test functions** — each TEST should test one logical scenario
- ❌ **Catch-all "everything works" tests** — one test per logical scenario
- ❌ **Hardcoded magic numbers without context** — use named constants or comments
- ❌ **Ignoring return values** — always capture and check return values
- ❌ **Redundant tests** — same code path tested multiple times with no new coverage

## Phase 6 — Multi-Agent Collaboration Patterns

### 6.1 Generate + Review Pattern

```
# Agent 1 (Build): Generate tests
"Generate Google Test unit tests for @include/module.h following the cpp-unit-test skill"

# Agent 2 (@test-reviewer or Plan mode): Review
"Review tests/test_module.cpp against the coverage matrix.
 Check for missing edge cases, fragile assertions, and anti-patterns.
 Rate coverage: [Complete/Partial/Minimal]"
```

### 6.2 Parallel Generation

```
# Generate tests for multiple modules in parallel using @general
"@general Generate tests for module_a, output to tests/test_module_a.cpp"
"@general Generate tests for module_b, output to tests/test_module_b.cpp"
```

### 6.3 Incremental Extension

```
# Step 1: Analyze existing coverage
"@explore List all public methods in include/*.h and check which have tests"

# Step 2: Fill gaps
"Generate tests only for untested methods identified above"
```

## Prompt Templates

### Template A: Full Module Test Generation

```
Follow the cpp-unit-test skill to generate Google Test unit tests for the module
defined in @include/<module>.h (implementation in @src/<module>.cpp).

Steps:
1. Read both header and implementation
2. Categorize each public method
3. Generate coverage matrix
4. Write tests to tests/test_<module>.cpp
5. Update tests/CMakeLists.txt to include the new test target

Requirements:
- Match exact API signatures from the header
- Use EXPECT_* for non-fatal, ASSERT_* only for preconditions
- Use EXPECT_DOUBLE_EQ / EXPECT_NEAR for floating-point
- Use structured bindings for pair/tuple returns
- Use ASSERT_TRUE(opt.has_value()) before dereferencing optional
- Add brief comments for non-obvious test scenarios
- Group tests by method with // === method_name === separators
```

### Template B: Extend Existing Tests

```
Read tests/test_<module>.cpp and @include/<module>.h.
Identify untested public methods and missing coverage rows from the cpp-unit-test
coverage matrix. Generate additional TEST cases ONLY for the gaps.
Do not modify existing tests.
```

### Template C: Test Quality Review

```
Review tests/test_<module>.cpp against the cpp-unit-test quality audit checklist.
For each finding, provide:
- Location (test name)
- Category (completeness/correctness/quality/design)
- Severity (blocker/warning/suggestion)
- Recommended fix (concrete code change)
```

### Template D: Fix Compilation Errors

```
Read tests/test_<module>.cpp and @include/<module>.h.
Fix all compilation errors by matching test code to actual API signatures.
Common issues to check:
- Wrong function signatures (guessed vs actual)
- Wrong return type handling (optional, variant, pair)
- Wrong exception types
- Missing includes
- Wrong namespace or class name
```
