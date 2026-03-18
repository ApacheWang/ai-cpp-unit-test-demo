# AI-CPP-UnitTest-Demo

> 用 OpenCode + Oh-My-OpenCode 演示 AI 辅助 C++ 单元测试生成

## 🎯 演示目的

演示如何利用 AI 编码工具（OpenCode + Oh-My-OpenCode）自动为 C++ 项目生成高质量的单元测试用例。

## 📁 项目结构

```
ai-cpp-unit-test-demo/
├── include/
│   ├── calculator.h       # 待测试模块 - 基础计算器
│   ├── string_utils.h     # 待测试模块 - 字符串工具
│   └── data_parser.h      # 待测试模块 - 数据解析器
├── src/
│   ├── calculator.cpp     # 计算器实现
│   ├── string_utils.cpp   # 字符串工具实现
│   └── data_parser.cpp    # 数据解析器实现
├── tests/
│   ├── CMakeLists.txt     # 测试的 CMake 配置
│   ├── test_calculator.cpp    # 单元测试
│   ├── test_string_utils.cpp  # 单元测试
│   └── test_data_parser.cpp   # 单元测试
├── .opencode/
│   └── skills/
│       └── cpp-unit-test/
│           └── SKILL.md   # 🔑 OpenCode Skill — C++ 单元测试生成规范
├── CMakeLists.txt         # 顶层 CMake 配置
├── AGENTS.md              # OpenCode Agent 指令
├── OPENCODE_GUIDE.md      # OpenCode 使用指南
└── README.md
```

## 🔑 OpenCode Skill: cpp-unit-test

本项目包含一个系统化的 OpenCode Skill，定义在 `.opencode/skills/cpp-unit-test/SKILL.md`。

**核心特性：**

- **Phase 0 — Pre-Flight Checks** — 读取头文件、分析实现、检查已有测试，绝不猜测 API
- **Phase 1 — Interface Analysis** — 方法分类（纯函数/有状态/查询/工厂/异常/错误码/模板）
- **Phase 2 — Coverage Matrix** — 系统化覆盖矩阵：正常路径、空/零输入、边界值、无效输入、数学属性
- **Phase 3 — Code Generation Rules** — 断言选择指南、Fixture 设计、参数化测试、类型参数化、gMock Matchers
- **Phase 4 — CMake Integration** — 测试 CMake 模板、编译运行命令
- **Phase 5 — Quality Audit Checklist** — 完整性/正确性/断言质量/设计反模式检查
- **Phase 6 — Multi-Agent Patterns** — Generate+Review、并行生成、增量扩展
- **Prompt Templates** — 全量生成、增量补充、质量审查、修复编译错误

**使用方式：** 在 OpenCode 中输入 prompt 时引用 skill 即可：
```
Follow the cpp-unit-test skill to generate tests for @include/calculator.h
```

## 🛠 环境准备

### 1. 安装 OpenCode

```bash
# macOS / Linux
brew install anomalyco/tap/opencode

# Windows (Scoop)
scoop install opencode

# 或使用 npm
npm i -g opencode-ai@latest
```

### 2. 安装 Oh-My-OpenCode (omo)

```bash
# omo 是 OpenCode 的 Agent 编排工具（原名 oh-my-opencode）
# 参考: https://github.com/code-yeongyu/oh-my-openagent
npx omo init
```

### 3. 安装编译工具

```bash
# 需要支持 C++17 的编译器 (GCC 7+ / Clang 5+ / MSVC 2017+)
# 以及 CMake 3.14+

# Ubuntu/Debian
sudo apt install cmake g++ libgtest-dev

# macOS
brew install cmake googletest

# Windows (vcpkg)
vcpkg install gtest
```

## 🚀 演示步骤

### Step 1: 查看源码

```bash
# 查看待测试的模块
cat include/calculator.h
cat src/calculator.cpp
```

### Step 2: 用 OpenCode 生成单元测试

```bash
# 启动 OpenCode
opencode

# 在 OpenCode 中输入以下 prompt：
# "请为 calculator 模块生成完整的 Google Test 单元测试，
#  覆盖所有公开函数和边界条件，输出到 tests/test_calculator.cpp"
```

### Step 3: 编译运行测试

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
ctest --output-on-failure
```

### Step 4: 用 Oh-My-OpenCode 多 Agent 协作

```bash
# 使用 omo 进行更复杂的测试生成场景
# - 一个 Agent 分析代码覆盖
# - 一个 Agent 生成边界用例
# - 一个 Agent 检查测试质量
```

## 📦 待测试模块说明

### Calculator (基础计算器)
- 四则运算（加减乘除）
- 链式调用
- 错误处理（除零、溢出）

### StringUtils (字符串工具)
- 大小写转换
- 字符串分割
- trim / 去空白
- 模式匹配

### DataParser (数据解析器)
- JSON-like 格式解析
- 键值对提取
- 类型转换
- 错误恢复

## 💡 演示要点

1. **Prompt 工程技巧** — 如何写好测试生成的 prompt
2. **上下文管理** — AGENTS.md 如何引导 AI 理解项目规范
3. **迭代优化** — 从基础测试到边界条件的渐进式生成
4. **多 Agent 协作** — omo 编排多个 Agent 各司其职
5. **质量把关** — 让 AI 自查测试覆盖率和断言质量

## 📄 License

MIT
