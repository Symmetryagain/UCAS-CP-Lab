# CACT Compiler

**English** | [**中文**](README_CN.md)

A compiler for **CACT** (_C A C T_), a simplified C-like language, targeting **RISC-V 64** (RV64). Built with C++17 and ANTLR4, the project covers the full compilation pipeline: lexing, parsing, semantic analysis, intermediate representation (IR) generation, and assembly code generation. A standalone IR interpreter is included for validation.

## Language Overview

CACT supports a subset of C with the following features:

| Feature | Support |
|---------|---------|
| Types | `int`, `float`, `double`, `bool`, `void` |
| Constants | `const` variables and arrays |
| Arrays | Multi-dimensional, with initialization |
| Operators | Arithmetic (`+` `-` `*` `/` `%`), unary (`-` `!`), comparison (`<` `>` `<=` `>=` `==` `!=`), logical (`&&` `\|\|`) |
| Control Flow | `if`/`else`, `while`, `break`, `continue` |
| Functions | Multi-parameter, recursive, `void` return type |
| Scoping | Lexical (block) scoping |
| Built-ins | `print_int`, `print_float`, `print_double`, `print_bool`, `get_int`, `get_float`, `get_double` |

## Compiler Pipeline

```
Source (.cact) → Lexer → Parser → Semantic Analyzer → IR → RISC-V Assembly (.s)
                                                    ↓
                                              IR Interpreter (for validation)
```

1. **Lexer & Parser** — ANTLR4-generated, based on `grammar/CACT.g4`
2. **Semantic Analysis** — Type checking, symbol resolution, scope management (Visitor pattern)
3. **IR Generation** — Custom three-address code IR (see `doc/IR_rules.md`)
4. **Assembly Generation** — Translates IR to RISC-V 64 assembly
5. **IR Interpreter** — Standalone tool that directly executes IR for testing and debugging

## Directory Structure

```
CACT05/
├── grammar/               ANTLR grammar source
│   └── CACT.g4
├── generated/grammar/     Generated lexer/parser/visitor (by ANTLR)
├── include/               Header files
│   ├── analyzer.h         Semantic analyzer (AST visitor)
│   ├── asm_gen.h          Assembly generator types
│   ├── btype.h            Base type definitions
│   ├── context.h          Compiler global state
│   ├── func_table.h       Function symbol table
│   ├── ir.h               IR opcode constants
│   ├── sym_table.h        Variable symbol table
│   ├── utils.h            Utility functions
│   └── utils_debug.h      AST/token debug printers
├── src/                   Source files
│   ├── main.cpp           Compiler entry point
│   ├── analyzer.cpp       Semantic analysis + IR generation
│   ├── asm_parse.cpp      IR text parser
│   ├── asm_codegen.cpp    RISC-V assembly code generation
│   ├── context.cpp        Global state definitions
│   ├── func_table.cpp     Function table implementation
│   ├── sym_table.cpp      Symbol table implementation
│   ├── utils.cpp          Utility implementations
│   └── interpreter/       IR interpreter
│       ├── main.cpp       Interpreter entry point
│       ├── exec.cpp       IR instruction execution
│       ├── init.cpp       Initialization and state management
│       ├── value.cpp      Value accessors
│       ├── type_conv.cpp  Type system conversions
│       └── util.cpp       String utilities and opcode parsing
├── lib/                   Runtime library
│   ├── cactio.c           Compact I/O library (for linking)
│   └── libcactio.c        Extended I/O library
├── deps/                  Dependencies
│   └── antlr4-runtime/    ANTLR4 C++ runtime
├── test/                  Test suites
│   ├── samples_lex_and_syntax/   Lexer/parser test cases
│   ├── samples_semantic/         Semantic analysis test cases
│   ├── samples_codegen_functional/   Full pipeline test cases
│   ├── samples_interpreter/      Interpreter test cases
│   ├── ir_files/                 Generated IR outputs
│   ├── test_syntax.sh            Syntax test runner
│   ├── test_semantic.sh          Semantic test runner
│   ├── test_ir.sh                IR + interpreter test runner
│   └── test_ir_o1.sh             IR + interpreter test runner (-O1)
├── doc/                   Documentation
│   ├── IR_rules.md        IR specification
│   └── *.md               Project reports
├── CMakeLists.txt         CMake build configuration
└── Makefile               Convenience build wrapper
```

## Prerequisites

- **C++ compiler** with C++17 support (GCC 8+ or Clang 7+)
- **CMake** >= 3.11
- **Java** (for ANTLR grammar generation only)
- **RISC-V GNU Toolchain** (`riscv64-unknown-elf-gcc`) — only needed for assembling and linking the generated `.s` files
- **Spike RISC-V Simulator** (`spike`) — only needed for running compiled binaries

## Build

```bash
# Generate grammar files (requires Java)
make grammar

# Build everything (compiler + interpreter)
make build

# Or build just the interpreter
make interpreter

# Clean and rebuild
make rebuild
```

Alternatively, use CMake directly:

```bash
mkdir -p build && cd build
cmake ..
make -j
```

This produces two executables:

| Binary | Path | Purpose |
|--------|------|---------|
| `compiler` | `build/compiler` | CACT → RISC-V assembly compiler |
| `interpreter` | `build/interpreter` | Direct IR executor |

## Usage

### Compiler

```bash
./build/compiler <input.cact> [-o output.s] [options]
```

Options:

| Flag | Description |
|------|-------------|
| `--syntax` | Stop after parsing (lexer + syntax check only) |
| `--emit-IR` | Generate IR only, skip assembly generation |
| `--print-tokens` | Print token stream to stderr |
| `--print-parser-tree` | Print CST (parse tree) to stderr |
| `--print-ast` | Print formatted AST to stderr |
| `-O1` | Enable optimizations (constant folding + dead code elimination) |
| `-o <file>` | Output file path (default: `<input>.s` or `<input>.ir`) |

### IR Interpreter

```bash
./build/interpreter <file.ir>
```

Executes the IR directly and prints the `main()` return value. Supports all IR instructions including IO built-ins.

### Running Compiled Programs

```bash
# Compile CACT to RISC-V assembly
./build/compiler program.cact -o program.s

# Assemble and link with the runtime library
riscv64-unknown-elf-gcc program.s -Llib -lcactio -o program.elf

# Run on Spike simulator
spike pk program.elf
```

## Testing

```bash
cd test

# Lexer and parser tests (27 cases)
bash test_syntax.sh

# Semantic analysis tests (39 cases)
bash test_semantic.sh

# IR generation + interpreter tests (78 cases)
bash test_ir.sh

# IR tests with -O1 optimization
bash test_ir_o1.sh
```

Test case naming convention: `NN_flag_description.cact`

- `true` — program is valid (compiler should return 0)
- `false` — program contains an error (compiler should return non-zero)

## IR Specification

The intermediate representation is a text-based three-address code. Each value (variable, temporary, function) has a unique name typed by prefix:

| Prefix | Type | Example |
|--------|------|---------|
| `%i_N` | int | `%i_1` |
| `%f_N` | float | `%f_2` |
| `%d_N` | double | `%d_3` |
| `%b_N` | bool | `%b_4` |
| `%v_N` | void | `%v_5` |
| `%a*_N` | array/pointer | `%ai_6` |

See `doc/IR_rules.md` for the complete IR specification.

## Optimization

When invoked with `-O1`, the compiler enables:

- **Constant folding** — compile-time evaluation of constant expressions
- **Dead code elimination** — removal of unused functions and variables
- **Peephole optimizations** — strength reduction (e.g., multiply by power-of-two → shift), immediate folding (e.g., `addi` for small immediates)
