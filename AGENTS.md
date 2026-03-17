# AGENTS.md — OpenCode Agent 指令

本文件为 OpenCode AI Agent 提供项目上下文和行为规范。

## 项目概述

这是一个 C++ 单元测试演示项目，用于展示 AI 辅助生成测试用例的能力。

## 技术栈

- **语言**: C++17
- **构建系统**: CMake 3.14+
- **测试框架**: Google Test (GTest)
- **编译器**: GCC 7+ / Clang 5+ / MSVC 2017+

## 代码规范

### 测试命名

- 测试文件: `test_<module_name>.cpp`
- 测试用例: 使用 `TEST(TestSuiteName, TestScenario)` 格式
- Fixture: 使用 `TEST_F( FixtureName, TestScenario)`

### 测试组织

每个被测模块的测试应覆盖：

1. **正常路径 (Happy Path)** — 典型输入，期望输出
2. **边界条件 (Edge Cases)** — 零、空、极大/极小值
3. **异常路径 (Error Path)** — 非法输入、错误处理
4. **数学/逻辑属性** — 如交换律、结合律、往返一致性等

### 断言选择

- 精确值比较: `EXPECT_EQ`, `EXPECT_TRUE/FALSE`
- 浮点数: `EXPECT_DOUBLE_EQ` 或 `EXPECT_NEAR`
- 异常: `EXPECT_THROW`
- 字符串: `EXPECT_EQ` (std::string)

### 测试质量标准

- 每个公开方法至少 3 个测试用例
- 不测试私有方法
- 测试之间相互独立，不依赖执行顺序
- 使用有意义的测试名称，描述期望行为而非实现细节

## Prompt 模板

### 生成单个模块的测试

```
请为 <module_name> 模块生成完整的 Google Test 单元测试。
源文件: include/<module_name>.h 和 src/<module_name>.cpp
输出文件: tests/test_<module_name>.cpp

要求：
1. 覆盖所有公开方法
2. 每个方法至少 3 个测试用例（正常、边界、异常）
3. 添加数学属性验证（如适用）
4. 使用有意义的中文注释说明每个测试场景
5. 仅输出 C++ 代码，不需要解释
```

### 追加边界测试

```
查看 tests/test_<module_name>.cpp 中已有的测试，
补充以下场景的测试用例：
1. 并发安全性（如适用）
2. 性能敏感的边界（大输入、嵌套结构）
3. 输入变异（特殊字符、Unicode、空指针等）
```

### 代码审查

```
审查 tests/test_<module_name>.cpp：
1. 检查是否有遗漏的边界条件
2. 是否存在脆弱的断言（如依赖具体实现细节）
3. 是否有重复或冗余的测试
4. 给出改进建议
```

## 文件结构约定

```
include/   — 头文件（.h），公开接口
src/       — 实现文件（.cpp）
tests/     — 测试文件（test_*.cpp）+ tests/CMakeLists.txt
build/     — 构建产物（已在 .gitignore 中排除）
```
