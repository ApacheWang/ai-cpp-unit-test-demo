# OpenCode + 多 Agent 协作：C++ 单元测试生成实战指南

> 本文以一个真实的 C++ 项目为例，演示如何用 OpenCode 及其多 Agent 体系高效生成单元测试。

---

## 一、OpenCode 是什么

[OpenCode](https://opencode.ai) 是一个开源的终端 AI 编码代理（coding agent），类似于 Cursor/Cline，但完全运行在终端里，支持多种 LLM 提供商。

**核心特性：**

- **终端原生** — TUI 界面，支持 WezTerm、Alacritty、Ghostty 等
- **多模型支持** — Anthropic、OpenAI、Google、本地模型，甚至 OpenCode Zen（官方托管）
- **Plan/Build 双模式** — 先规划再动手，避免 AI 直接乱改代码
- **内置子 Agent** — `@general` 做研究、`@explore` 做代码搜索
- **自定义 Agent** — 通过配置文件创建专门的 Agent（如 code reviewer）
- **MCP 扩展** — 支持 Model Context Protocol，接入外部工具

### 安装

```bash
# macOS / Linux
brew install anomalyco/tap/opencode

# Windows (Scoop)
scoop install opencode

# npm
npm install -g opencode-ai
```

### 配置提供商

启动后运行 `/connect`，选择提供商并配置 API Key。推荐新手使用 OpenCode Zen（官方托管，按量计费）。

配置文件位置（优先级从低到高）：

| 位置 | 用途 |
|------|------|
| `~/.config/opencode/opencode.json` | 全局配置 |
| 项目根目录 `opencode.json` | 项目配置（优先级最高） |

示例配置：

```json
{
  "$schema": "https://opencode.ai/config.json",
  "model": "anthropic/claude-sonnet-4-20250514",
  "small_model": "anthropic/claude-haiku-4-20250514",
  "provider": {
    "anthropic": {
      "options": {
        "timeout": 600000
      }
    }
  }
}
```

---

## 二、Agent 体系详解

OpenCode 的 Agent 分为两类：

### 2.1 主 Agent（Primary Agents）

通过 **Tab 键** 切换：

| Agent | 用途 | 权限 |
|-------|------|------|
| **Build** | 默认模式，执行实际代码修改 | 读写文件、执行命令 |
| **Plan** | 分析、规划、建议 | 只读，修改需确认 |

### 2.2 子 Agent（Subagents）

通过 **@ 提及** 调用：

| Agent | 用途 |
|-------|------|
| **@general** | 通用研究，可修改文件 |
| **@explore** | 快速代码搜索，只读 |

### 2.3 自定义 Agent

可以在 `opencode.json` 中定义，也可以在 `.opencode/agents/` 目录下用 Markdown 文件定义：

**JSON 方式：**

```json
{
  "agent": {
    "code-reviewer": {
      "description": "Reviews code for best practices and potential issues",
      "mode": "subagent",
      "model": "anthropic/claude-sonnet-4-20250514",
      "prompt": "You are a code reviewer. Focus on security, performance, and maintainability.",
      "tools": {
        "write": false,
        "edit": false
      }
    }
  }
}
```

**Markdown 方式** (`.opencode/agents/review.md`)：

```markdown
---
description: Reviews code for quality and best practices
mode: subagent
model: anthropic/claude-sonnet-4-20250514
temperature: 0.1
tools:
  write: false
  edit: false
  bash: false
---

You are in code review mode. Focus on:
- Code quality and best practices
- Potential bugs and edge cases
- Performance implications
- Security considerations
```

### 2.4 温度控制

不同任务适合不同的温度：

```json
{
  "agent": {
    "plan": { "temperature": 0.1 },      // 严谨的规划
    "build": { "temperature": 0.3 },     // 平衡的编码
    "brainstorm": { "temperature": 0.7 } // 创意的头脑风暴
  }
}
```

---

## 三、AGENTS.md：给 AI 的项目说明书

`AGENTS.md` 是 OpenCode 最重要的上下文文件。放在项目根目录，OpenCode 启动时会自动读取（或通过 `/init` 命令生成）。

**它的作用：** 告诉 AI 你的项目是什么、怎么组织、代码规范、测试标准等。

### 示例：C++ 测试项目的 AGENTS.md

```markdown
# AGENTS.md — OpenCode Agent 指令

## 项目概述
C++17 单元测试演示项目，使用 CMake + Google Test。

## 代码规范

### 测试命名
- 测试文件: `test_<module_name>.cpp`
- 测试用例: `TEST(TestSuiteName, TestScenario)`
- Fixture: `TEST_F(FixtureName, TestScenario)`

### 测试覆盖要求
每个被测模块的测试应覆盖：
1. **正常路径 (Happy Path)** — 典型输入，期望输出
2. **边界条件 (Edge Cases)** — 零、空、极大/极小值
3. **异常路径 (Error Path)** — 非法输入、错误处理
4. **数学/逻辑属性** — 如交换律、结合律、往返一致性

### 断言选择
- 精确值: `EXPECT_EQ`, `EXPECT_TRUE/FALSE`
- 浮点数: `EXPECT_DOUBLE_EQ` 或 `EXPECT_NEAR`
- 异常: `EXPECT_THROW`
```

---

## 四、实战：用 OpenCode 生成 C++ 单元测试

### 4.1 项目结构

```
ai-cpp-demo/
├── include/
│   ├── calculator.h       # 基础计算器
│   ├── string_utils.h     # 字符串工具
│   └── data_parser.h      # 数据解析器
├── src/                   # 实现文件
├── tests/                 # 测试文件
├── CMakeLists.txt         # 构建 + FetchContent 自动下载 GTest
└── AGENTS.md              # Agent 指令
```

### 4.2 交互式生成（推荐）

```bash
cd ai-cpp-demo
opencode
```

在 TUI 中：

```
你> 请为 calculator 模块生成完整的 Google Test 单元测试，
    覆盖所有公开函数和边界条件，输出到 tests/test_calculator.cpp
```

OpenCode 会：
1. 读取 `include/calculator.h` 和 `src/calculator.cpp`
2. 读取 `AGENTS.md` 了解项目规范
3. 生成 `tests/test_calculator.cpp`
4. 用 LSP 检查语法（如果没有构建系统，LSP 报错可忽略）

### 4.3 非交互式生成（CI/自动化）

```bash
# 使用 opencode run 一次性执行
opencode run -m "anthropic/claude-sonnet-4-20250514" \
  "请为 StringUtils 模块生成完整的 Google Test 单元测试，写入 tests/test_string_utils.cpp。覆盖所有公开方法，每个方法至少3个测试用例"

# 继续上次会话
opencode run -c "补充更多边界测试用例"

# 指定 session
opencode run -s <session-id> "修复失败的测试"
```

### 4.4 Plan → Build 工作流

这是 OpenCode 推荐的完整工作流：

```
1. 按 Tab 切换到 Plan 模式
2. 描述需求：
   > 分析 DataParser 模块，制定测试计划，覆盖所有公开方法

3. AI 输出计划后，审核并确认：
   > 计划看起来不错，但 parseIni 需要增加注释测试场景

4. 按 Tab 切换到 Build 模式
5. 执行：
   > 按计划生成 tests/test_data_parser.cpp

6. 如有 LSP 错误，让 AI 修复：
   > 修复 LSP 报告的错误
```

### 4.5 编译验证

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
ctest --output-on-failure
```

---

## 五、多 Agent 协作工作流

对于复杂项目，可以利用 OpenCode 的多 Agent 能力实现并行测试生成和质量保障。

### 5.1 方案一：自定义 Reviewer Agent

定义一个测试审查 Agent，让 Build 生成测试后自动审查：

```json
{
  "agent": {
    "test-reviewer": {
      "description": "审查生成的单元测试质量",
      "mode": "subagent",
      "model": "anthropic/claude-sonnet-4-20250514",
      "prompt": "你是一个测试审查专家。检查：\n1. 是否覆盖所有公开方法\n2. 每个方法是否有正常/边界/异常三类用例\n3. 断言是否准确匹配实际 API 签名\n4. 是否存在冗余测试\n5. 给出具体改进建议",
      "tools": {
        "write": false,
        "edit": false,
        "bash": false
      }
    }
  }
}
```

使用：

```
你> @test-reviewer 审查 tests/test_data_parser.cpp 的测试质量
```

### 5.2 方案二：@explore + Build 串联

```
你> @explore 找到 src/ 下所有需要生成测试的模块

你> @explore 检查 tests/ 目录下已有的测试文件

你> 基于分析结果，为缺少测试的模块逐一生成测试
```

### 5.3 方案三：Oh-My-OpenCode (omo) 多 Agent 编排

[Oh-My-OpenCode](https://github.com/code-yeongyu/oh-my-openagent)（npm 包名 `omo`）是一个独立的多 Agent 编排工具，可以同时启动多个 OpenCode Agent 并行工作：

```bash
# 初始化 omo
npx omo init

# 多 Agent 并行生成测试
# - Agent A: 分析代码覆盖率缺口
# - Agent B: 生成 calculator 测试
# - Agent C: 生成 string_utils 测试
# - Agent D: 生成 data_parser 测试
```

omo 的核心思想是：**一个 Agent 做规划，多个 Agent 并行执行，最后汇总结果**。

---

## 六、工具权限管理

在生成测试时，合理配置工具权限很重要：

```json
{
  "permission": {
    "read": "allow",
    "grep": "allow",
    "glob": "allow",
    "write": "allow",
    "edit": "allow",
    "bash": "ask"
  }
}
```

- `allow` — AI 可以自由使用
- `ask` — 每次使用前需要你确认（推荐给 `bash`）
- `deny` — 完全禁止

---

## 七、实用技巧

### 7.1 引用文件

在 prompt 中用 `@` 引用特定文件，让 AI 精确读取：

```
请根据 @include/calculator.h 的接口定义，生成测试到 tests/test_calculator.cpp
```

### 7.2 @file 引入外部 Prompt

在配置中引用外部 prompt 文件：

```json
{
  "agent": {
    "build": {
      "prompt": "{file:./prompts/build.txt}"
    }
  }
}
```

### 7.3 /undo 和 /redo

AI 改错了？不用手动 revert：

```
/undo   # 撤销上次修改
/redo   # 重做
```

### 7.4 会话管理

```
/sessions    # 查看所有会话
# 用方向键导航子会话：↑ 父会话，↓ 第一个子会话，←→ 切换子会话
```

### 7.5 模型切换

```
/models   # 打开模型选择器
```

可以为不同 Agent 配置不同模型，比如 Plan 用便宜快速的模型，Build 用最强的模型。

---

## 八、完整工作流总结

```
┌─────────────────────────────────────────────┐
│               1. 项目初始化                    │
│  opencode → /init → 生成 AGENTS.md           │
├─────────────────────────────────────────────┤
│               2. 规划阶段 (Plan)              │
│  Tab 切换到 Plan → 描述测试需求 → 审核计划     │
├─────────────────────────────────────────────┤
│               3. 执行阶段 (Build)             │
│  Tab 切换到 Build → 按计划生成测试文件          │
├─────────────────────────────────────────────┤
│               4. 质量保障                      │
│  @test-reviewer 审查 → 修复 → 迭代            │
├─────────────────────────────────────────────┤
│               5. 编译验证                      │
│  cmake + ctest → 确认全部通过                 │
├─────────────────────────────────────────────┤
│               6. /share 分享                   │
│  生成会话链接 → 与团队共享 prompt 和结果        │
└─────────────────────────────────────────────┘
```

---

## 九、参考链接

- **OpenCode 官网**: https://opencode.ai
- **OpenCode 文档**: https://opencode.ai/docs
- **OpenCode GitHub**: https://github.com/anomalyco/opencode
- **Oh-My-OpenCode (omo)**: https://github.com/code-yeongyu/oh-my-openagent
- **OpenCode 配置 Schema**: https://opencode.ai/config.json
- **Agent 配置文档**: https://opencode.ai/docs/agents
- **工具权限文档**: https://opencode.ai/docs/tools
