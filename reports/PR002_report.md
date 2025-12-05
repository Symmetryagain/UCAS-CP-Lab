# PR002 CACT 实验报告

## 0. 小组成员

- 胡格致
- 石曜铭
- 蔡合森

在之前的PR001的基础上，我们完成了CACT语言的语义分析与中间表示生成，并实现了可以执行中间表示的模拟器。

## 1. 语义分析和中间表示生成

### 1.1 ANTLR4 语法文件

首先，我们重写了实验一中的ANTLR语法文件，浓缩了仅仅因为类型不同而产生的多个parser，将类型检查的工作挪到了语义分析阶段。
我们还修复了语法检查测试样例26未通过的问题。

在此基础上，我们通过给parser加入属性变量，为之后语义检查和中间变量生成打下了基础。

### 1.2 ANTLR4 Visitor

我们通过使用ANTLR4生成的Visitor模式，从 `comp_units` 开始，递归地完成如下作用

1. **符号表管理**：使用 `SymTree` 管理变量符号，`FuncTable` 管理函数符号，支持作用域嵌套
2. **属性计算与传递**：在遍历语法树时，计算并传递结点的属性（如类型、数组维度、结果变量等）
3. **语义检查**：在属性计算过程中进行语义检查
4. **IR生成**：在通过语义检查后，立即输出对应的IR指令

### 1.2.1 属性

在语法分析树的每个结点上，我们设计了以下属性：

- **btype**：表示表达式的类型（Int, Float, Double, Bool, Void）。
- **array_size**：如果结点表示数组，则存储数组各维度的大小（vector<size_t>）；否则为空。
- **res**：表示该表达式计算结果的 IR 临时变量名（如 "%t1"）。
- **value**：对于常量结点，存储其字符串形式的常数值。
- **sign**：对于有符号常量，表示是否有负号。
- **code**：对于左值（L_value），存储其在 IR 中对应的变量名或数组元素地址。
- **need_type**：在函数返回、变量声明等上下文中，表示期望的类型。
- **has_return**：表示当前块中是否包含 return 语句。
- **brk_target** 和 **ctn_target**：在循环语句中，分别表示 break 和 continue 跳转的目标标签。

#### 1.2.2 语义检查

我们在语义分析中，主要进行了以下的检查：

- **变量/函数重复定义**：通过符号表（`g_symtree`）和函数表（`g_functable`）检查标识符是否已定义。
- **类型匹配**：在赋值、运算、函数调用、返回等场景中检查类型是否一致。
- **数组维度匹配**：数组访问时下标个数与数组维度一致，且下标表达式为整型。
- **常量修改**：禁止对常量进行赋值。
- **控制流语句合法性**：break 和 continue 必须出现在循环体内。
- **函数调用参数匹配**：参数个数、类型、数组维度必须与函数定义一致。
- **main 函数特殊要求**：返回类型必须为 Int，且不能有参数。

#### 1.2.3 中间表示生成

我们在语义分析的基础上，生成了中间表示（IR）指令。具体翻译过程如下：

- **变量声明**：遇到 `var_def` 结点时，在符号表中定义变量，输出 `@var` 或 `@array` 指令
- **表达式计算**：对算术表达式，先递归计算子表达式，生成临时变量存储中间结果，输出运算指令
- **控制流语句**：为if、while等生成标签和条件跳转指令
- **函数调用**：计算实参，输出 `call` 指令
- **返回**：生成 `return` 指令

## 2. IR 语法

### 2.1 局部变量

- 非数组（数据、指针）：每个变量名都是形如 类型_id 的格式，例如 `%i_1`, `%d_3`, `%f_4`, `%b_7`，分别表示一个 int, double, float, bool 类型。
    - 声明：`@var 变量名`，如 `@var %d_1`, `@var %ai_2`。
- 数组（内存申请）：
    - 定义：数组名的格式是形如 `%ai_5`，这也是指针的格式。数组定义的格式是 `@array 数组名 size`，例如 `@array %ai_5 5`，表示大小为 5 的 int 数组 ai_5。
    - 原代码中局部数组定义如果带赋值，则拆成先定义再赋值。
    - 访问：
    - 得到数组某处的指针：`Addr dest base offset`表示将 base+offset 存入dest，表示一个指针 
    - 得到数组在某处的值：`base[offset]`，例如 `%ad_6[%i_7]`或 `%ad_6[3]`

### 2.2 全局变量

- 声明时，在对应的变量声明语句前添加 `!global` 前缀即可。**局部变量和全局变量均拆成先定义后赋值**

### 2.3 函数

- 定义格式：`@func 函数名 (参数1, 参数2, ...) 函数体 @endfunc`。其中，函数名格式为 返回值类型_id（void 为 v）。参数可以是一个数据类型，也可以是一个指针。例如：`@func %d_9 (%i_10, %f_11, %ad_12, %b_14)` 表示 `double d_9(int a, float b, double *c, bool *d)`。这里指针对应原代码中的数组，我们把任何维度的数组都展平成一维数组处理。
- 调用格式：
    - `void` ：`call void 函数名 ( 参数1 , 参数2 , ... )`
    - 非 `void`：`call 变量名 函数名 ( 参数1 , 参数2 , ... )`
- 特殊函数：`print_int` 等库函数，在生成中间代码时直接 call，模拟器中按含义模拟即可；对于 `main` 函数，不会进行重命名，中间代码中的名字仍然为 `main`。

### 2.4 指令

- 三地址代码。
    - 运算。形如 `op [type] dest src1 src2`，表示将 `src1 op src2` 的结果存入 `dest` 中。`op` 可能是 `DAnd, DOr, Mul, Add, Sub, Div, Rem, LT, GT, LE, GE, EQ, NE`。对于可能多种数据类型运算的指令，使用`[type]`标注，例如 `DOr b_14 b_15 b_16` 表示 `b_14 = b_15 || b_16`，`Div d d_17 d_18 3.0` 表示 `d_17 = d_18 / 3.0`。
    - `Addr`。形如 `Addr dest base offset`，其中 `base` 是一个指针，表示将 `base+offset` 存入 `dest`。
- 二地址代码。
    - 运算。形如 `op [type] dest src`。表示将 `op src` 存入 `dest`，其中 `op` 可能是 `Neg Not`，如 `Not b_20 b_21` 表示 `b_20 = !b_21`；`Neg i %i_2 %i_3` 表示 `i_2 = -%i_3`。
    - 赋值。形如 `assign dest src`，表示 `dest = src`。
    - 跳转。形如 `branch target condition`，表示如果 `condition` 值为 `true`，则跳转到 label `target` 的下一行处。例如 `branch label_1 %b_20`，表示如果 `b_20` 为 `true` 则跳转到 `label_1` 的下一行代码。
- 一地址代码。
    - 标签。形如 `label label_name`。表示创建一个名字叫 `label_name` 的标签，保证一个 IR 代码中标签名字不重复。
    - 返回值。形如 `return var_name`，用于函数内部，表示将 `var_name` 变量的值返回；如果没有返回值则为 `return`。
    - 变量退出作用域。形如 `retire var_name`，表示该变量名之后不会再使用，用于优化生成汇编代码中的寄存器调度。在模拟器中可以省略。

**保证所有 id 均互不相同，因此任意两个名字都不同。**

### 2.5 例子

以下是 IR 代码的例子：

1.

```c
int main()
{
    int a = 2;
    int b;
    b = -2;
    int c;
    c = +b;
    bool d = false;
    bool e;
    e = !d;
    return c;
}
```

```ir
@func %main (  )
    assign %i_1 2
    assign %i_2 -2
    Add i %i_3 %i_2
    assign %b_4 false
    Not %b_5 %b_4
    return %i_3
@endfunc
```

2.

```c
double foo(double x[2], double y[2]){
    return x[0] + y[0];
}

int main() {
    double a[2][2] = { {1.0, 2.0}, {4.5e-2} };
    foo( a[0], a[1] );
    return 0;
}
```

```ir
@func %d_1 ( %ad_2 , %ad_3 ) 
  Add d %d_4 %ad_2[0] %ad_3[0]
  return %d_4
@endfunc
@func %main ( )
  @array %ad_5 4
  assign %ad_5[0] 1.0
  assign %ad_5[1] 2.0
  assign %ad_5[2] 4.5e-2
  assign %ad_5[3] 0
  Addr %ad_6 %ad_5 0
  Addr %ad_7 %ad_5 2
  call %d_8 %d_1 ( %ad_6 , %ad_7 )
  return 0
@endfunc
```

3.

```c
void func(int a)
{
    a = 2;
    int b = 3;
    a = a + b;
    return ;
}

int main()
{
    int a = 4;
    int b = 2;
    func(b);
    return a;
}
```

```ir
@func %v_1 ( %i_2 ) 
  assign %i_2 2
  assign %i_3 3
  Add i %i_2 %i_2 %i_3
  return
@endfunc
@func %main ( )
  assign %i_4 4
  assign %i_5 2
  call void %v_1 ( %i_5 )
  return %i_4
@endfunc
```

4.

```c
int main()
{
    int i = 0;
    int b = 0;
    while(i < 3){
        int b = 1;
        i = i + b;
    }

    {
        const int b = 2;
    }
    return b;
}
```

```ir
@func %main ( )
  assign %i_1 0
  assign %i_2 0
  label l_3
  LT i %b_4 %i_1 3
  branch l_5 %b_3
  assign %i_6 1
  Add i %i_1 %i_1 %i_6
  retire %i_6
  branch l_3 1
  label l_5
  assign %i_7 2
  retire %i_7
  return %i_2
 @endfunc
```

**注：IR 代码的不同部分之间加了充足的空格，方便输入给下游程序。**

## 3 解释器实现

解释器位于 `interpreter/` 目录，是一个独立的程序，读取 IR 文件并解释执行。

### 3.1 数据结构

在实现ir的解释器中，我们定义了以下数据结构：

- **Ident**：表示符号表中的条目，包含名称、类型（IdentType）和值（Value 联合体）。
- **Table**：符号表，为 `map<string, Ident>`。
- **Local_State**：表示函数局部状态，包括当前行号、返回类型和局部符号表。

**关键数据结构定义**（来自 `interpreter.h`）：

```cpp
enum IdentType {
    IT_NONE, IT_INT, IT_FLOAT, IT_DOUBLE, IT_BOOL,
    IT_ARRAY_INT, IT_ARRAY_FLOAT, IT_ARRAY_DOUBLE, IT_ARRAY_BOOL,
    IT_FUNC_INT, IT_FUNC_FLOAT, IT_FUNC_DOUBLE, IT_FUNC_BOOL, IT_FUNC_VOID
};

enum OpType {
    OP_EMPTY, OP_FUNC_DEF, OP_FUNC_END, OP_FUNC_CALL,
    OP_G_VAR, OP_G_ASSIGN, OP_G_ARRAY, OP_VAR, OP_ASSIGN, OP_ARRAY,
    OP_AND, OP_OR, OP_MUL, OP_ADD, OP_ADDR, OP_SUB, OP_DIV, OP_REM,
    OP_LT, OP_GT, OP_LE, OP_GE, OP_EQ, OP_NE, OP_POS, OP_NEG, OP_NOT,
    OP_BRANCH, OP_LABEL, OP_RETURN, OP_RETIRE
};

union Value {
    int    line;
    int    ival;
    float  fval;
    double dval;
    bool   bval;
    Value *aval; // 用于数组+

};
```

解释器通过 `gtable`（全局符号表）和 `ltable`（局部符号表）管理变量，支持多种数据类型和数组类型。

#### 3.2 解释执行过程

1. **初始化**：读取 IR 文件，将每行存入 `lstrs` 向量。
2. **全局初始化**：扫描 IR，处理全局变量/数组声明（`@var`、`@array`）、函数定义（`@func`）等，建立全局符号表。
3. **执行主函数**：调用 `func_call("%main", {})`，进入主函数解释执行。
4. **指令执行**：根据每行 IR 的操作码（OpType）调用相应的处理函数（如 `op_add`、`op_branch` 等），更新符号表和程序状态。
5. **函数调用**：保存当前状态，创建新的局部符号表，执行函数体，返回后恢复现场。

解释器支持所有 IR 指令，并能正确处理类型转换、数组访问、控制流和函数调用。

## 4. 测试与验证

### 4.1 语义分析测试

我们使用测试脚本 `test/test_semantic.sh` 运行编译器，对 `test/samples_semantic/` 下的测试用例（共 39 个），通过编译器检查语义错误。我们的编译器成功对所有的样例进行了正确的判断。

### 4.2 IR 生成与解释执行测试

我们使用 `test/samples_codegen_functional/` 的部分样例和 `test/samples_interpreter/` 的全部样例。
通过编译 CACT 程序生成 IR，然后运行解释器执行 IR，比较输出结果与对应的 C 程序输出是否一致。

在我们测试的样例中，未发现执行上的差异。

## 总结与心得体会

通过本次实验，我们深入理解了龙书中的属性计算方法和中间表示生成方法。
我们设计了自定义的 IR 并实现了完整的解释器，能够将 CACT 源码编译成 IR 并解释执行。

实验过程中遇到的主要挑战包括：

1. **类型系统的复杂性**：CACT 支持多种基本类型和数组，需要仔细处理类型匹配。
2. **数组语义**：数组作为参数传递时维度的处理、数组初始化等细节较多。
3. **控制流语句的 IR 生成**：if-else、while、break、continue 的跳转标签管理需要精心设计。

通过本次实验，我们不仅加深了对编译原理的理解，也锻炼了系统设计和调试能力。后续可以进一步优化错误信息输出、增加更多编译优化等。

## 附录

### 项目结构

```tree
cplab/
├── grammar/          # 语法文件
├── include/          # 头文件（Analyzer.h, btype.h, SymTable.h, FuncTable.h）
├── src/              # 编译器源码（Analyzer.cpp, main.cpp 等）
├── interpreter/      # 解释器源码
├── test/             # 测试用例
└── reports/          # 实验报告
```

### 编译与运行

1. 生成语法分析器：`make grammar`
2. 构建项目：`make build`
3. 运行编译器：`./build/compiler <cact-file> > <ir-file>`
4. 构建解释器：`make -C ./interpreter`
5. 运行解释器：`./interpreter/interpreter <ir-file> > <out-file>`

---
报告撰写日期：2025年12月3日
