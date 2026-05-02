# CACT 编译器

[**English**](README.md) | **中文**

CACT (_C A C T_) 语言的编译器，CACT 是一种简化的类 C 语言。目标架构为 **RISC-V 64** (RV64)。项目使用 C++17 和 ANTLR4 构建，覆盖完整的编译流水线：词法分析、语法分析、语义分析、中间表示（IR）生成以及汇编代码生成。同时包含一个独立的 IR 解释器用于验证。

## 语言概览

CACT 支持 C 语言的一个子集，包含以下特性：

| 特性 | 支持内容 |
|------|---------|
| 数据类型 | `int`、`float`、`double`、`bool`、`void` |
| 常量 | `const` 变量和数组 |
| 数组 | 多维数组，支持初始化 |
| 运算符 | 算术 (`+` `-` `*` `/` `%`)、一元 (`-` `!`)、比较 (`<` `>` `<=` `>=` `==` `!=`)、逻辑 (`&&` `\|\|`) |
| 控制流 | `if`/`else`、`while`、`break`、`continue` |
| 函数 | 多参数、递归、`void` 返回类型 |
| 作用域 | 词法（块）作用域 |
| 内置函数 | `print_int`、`print_float`、`print_double`、`print_bool`、`get_int`、`get_float`、`get_double` |

## 编译器流水线

```
源码 (.cact) → 词法分析 → 语法分析 → 语义分析 → IR  → RISC-V 汇编 (.s)
                                              ↓
                                        IR 解释器 (用于验证)
```

1. **词法与语法分析** — 基于 `grammar/CACT.g4`，由 ANTLR4 生成
2. **语义分析** — 类型检查、符号解析、作用域管理（使用 Visitor 模式）
3. **IR 生成** — 自定义的三地址码中间表示（参见 `doc/IR_rules.md`）
4. **汇编生成** — 将 IR 翻译为 RISC-V 64 汇编
5. **IR 解释器** — 独立工具，可直接执行 IR 用于测试和调试

## 目录结构

```
CACT05/
├── grammar/               ANTLR 语法源文件
│   └── CACT.g4
├── generated/grammar/     生成的词法/语法/访问器（由 ANTLR 产生）
├── include/               头文件
│   ├── analyzer.h         语义分析器（AST 访问器）
│   ├── asm_gen.h          汇编生成器类型定义
│   ├── btype.h            基础类型定义
│   ├── context.h          编译器全局状态
│   ├── func_table.h       函数符号表
│   ├── ir.h               IR 操作码常量
│   ├── sym_table.h        变量符号表
│   ├── utils.h            工具函数
│   └── utils_debug.h      AST/Token 调试打印器
├── src/                   源文件
│   ├── main.cpp           编译器入口
│   ├── analyzer.cpp       语义分析 + IR 生成
│   ├── asm_parse.cpp      IR 文本解析器
│   ├── asm_codegen.cpp    RISC-V 汇编代码生成
│   ├── context.cpp        全局状态定义
│   ├── func_table.cpp     函数表实现
│   ├── sym_table.cpp      符号表实现
│   ├── utils.cpp          工具函数实现
│   └── interpreter/       IR 解释器
│       ├── main.cpp       解释器入口
│       ├── exec.cpp       IR 指令执行
│       ├── init.cpp       初始化与状态管理
│       ├── value.cpp      值访问器
│       ├── type_conv.cpp  类型系统转换
│       └── util.cpp       字符串工具与操作码解析
├── lib/                   运行时库
│   ├── cactio.c           紧凑 I/O 库（用于链接）
│   └── libcactio.c        扩展 I/O 库
├── deps/                  依赖项
│   └── antlr4-runtime/    ANTLR4 C++ 运行时
├── test/                  测试套件
│   ├── samples_lex_and_syntax/   词法/语法测试用例
│   ├── samples_semantic/         语义分析测试用例
│   ├── samples_codegen_functional/   完整流水线测试用例
│   ├── samples_interpreter/      解释器测试用例
│   ├── ir_files/                 生成的 IR 输出
│   ├── test_syntax.sh            语法测试脚本
│   ├── test_semantic.sh          语义测试脚本
│   ├── test_ir.sh                IR + 解释器测试脚本
│   └── test_ir_o1.sh             IR + 解释器测试脚本 (-O1)
├── doc/                   文档
│   ├── IR_rules.md         IR 规范
│   └── *.md               项目报告
├── CMakeLists.txt         CMake 构建配置
└── Makefile               便捷构建包装
```

## 环境要求

- **C++ 编译器** 支持 C++17（GCC 8+ 或 Clang 7+）
- **CMake** >= 3.11
- **Java**（仅用于 ANTLR 语法文件生成）
- **RISC-V GNU 工具链** (`riscv64-unknown-elf-gcc`) — 仅用于汇编和链接生成的 `.s` 文件
- **Spike RISC-V 模拟器** (`spike`) — 仅用于运行编译后的二进制文件

## 构建

```bash
# 生成语法文件（需要 Java）
make grammar

# 构建全部（编译器 + 解释器）
make build

# 或仅构建解释器
make interpreter

# 清理并重新构建
make rebuild
```

也可直接使用 CMake：

```bash
mkdir -p build && cd build
cmake ..
make -j
```

构建产物：

| 二进制文件 | 路径 | 用途 |
|-----------|------|------|
| `compiler` | `build/compiler` | CACT → RISC-V 汇编编译器 |
| `interpreter` | `build/interpreter` | 直接执行 IR |

## 使用方式

### 编译器

```bash
./build/compiler <input.cact> [-o output.s] [options]
```

选项：

| 选项 | 说明 |
|------|------|
| `--syntax` | 仅解析（词法 + 语法检查） |
| `--emit-IR` | 仅生成 IR，跳过汇编生成 |
| `--print-tokens` | 将 Token 流打印到 stderr |
| `--print-parser-tree` | 将 CST（解析树）打印到 stderr |
| `--print-ast` | 将格式化 AST 打印到 stderr |
| `-O1` | 启用优化（常量折叠 + 死代码消除） |
| `-o <file>` | 输出文件路径（默认：`<input>.s` 或 `<input>.ir`） |

### IR 解释器

```bash
./build/interpreter <file.ir>
```

直接执行 IR 并打印 `main()` 的返回值。支持所有 IR 指令，包括 IO 内置函数。

### 运行编译后的程序

```bash
# 将 CACT 编译为 RISC-V 汇编
./build/compiler program.cact -o program.s

# 汇编并链接运行时库
riscv64-unknown-elf-gcc program.s -Llib -lcactio -o program.elf

# 在 Spike 模拟器上运行
spike pk program.elf
```

## 测试

```bash
cd test

# 词法和语法测试（27 个用例）
bash test_syntax.sh

# 语义分析测试（39 个用例）
bash test_semantic.sh

# IR 生成 + 解释器测试（78 个用例）
bash test_ir.sh

# 带 -O1 优化的 IR 测试
bash test_ir_o1.sh
```

测试用例命名约定：`NN_flag_description.cact`

- `true` — 程序合法（编译器应返回 0）
- `false` — 程序含有错误（编译器应返回非 0）

## IR 规范

中间表示为基于文本的三地址码。每个值（变量、临时变量、函数）都有一个按类型前缀区分的唯一名称：

| 前缀 | 类型 | 示例 |
|------|------|------|
| `%i_N` | int | `%i_1` |
| `%f_N` | float | `%f_2` |
| `%d_N` | double | `%d_3` |
| `%b_N` | bool | `%b_4` |
| `%v_N` | void | `%v_5` |
| `%a*_N` | 数组/指针 | `%ai_6` |

完整的 IR 规范参见 `doc/IR_rules.md`。

## 优化

使用 `-O1` 调用时，编译器启用：

- **常量折叠** — 编译期计算常量表达式
- **死代码消除** — 移除未使用的函数和变量
- **窥孔优化** — 强度削弱（如 2 的幂乘法 → 移位）、立即数折叠（如小立即数用 `addi`）
