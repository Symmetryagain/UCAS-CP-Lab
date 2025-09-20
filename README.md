# CACT 实验框架

最近更新：2025 年 9 月 20 日。

## 目录结构

deps: ANTLR 工具及其 C++ 库，不要修改。

grammar: 语法文件（.g4）以及生成的语法分析器，**不要修改除 .g4 文件之外的文件**。

src: CACT 编译器相关的源文件，建议同学们将自己编写的代码置于此目录。

test: 各个层次的测试程序。

## 构建运行步骤

### 使用 ANTLR 生成语法分析器

推荐使用 C++ 语言，Visitor 模式。

```bash
$ cd grammar
$ java -jar ../deps/antlr-4.13.1-complete.jar -Dlanguage=Cpp CACT.g4 -visitor -no-listener
```

### 构建整个项目

```bash
$ mkdir -p build
$ cd build
$ cmake ..
$ make -j
```

### 运行测试

```bash
$ ./compiler ../test/samples_lex_and_syntax/00_true_main.cact
```

