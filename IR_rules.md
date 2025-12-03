# IR 语法

1. 局部变量
    - 非数组（数据、指针）：每个变量名都是形如 类型_id 的格式，例如 `%i_1`, `%d_3`, `%f_4`, `%b_7`，分别表示一个 int, double, float, bool 类型。
      - 声明：`@var 变量名`，如 `@var %d_1`, `@var %ai_2`。
    - 数组（内存申请）：
      - 定义：数组名的格式是形如 `%ai_5`，这也是指针的格式。数组定义的格式是 `@array 数组名 size`，例如 `@array %ai_5 5`，表示大小为 5 的 int 数组 ai_5。
      - 原代码中局部数组定义如果带赋值，则拆成先定义再赋值。
      - 访问：
        - 得到数组某处的指针：`Addr dest base offset`表示将 base+offset 存入dest，表示一个指针 
        - 得到数组在某处的值：`base[offset]`，例如 `%ad_6[%i_7]`或 `%ad_6[3]`
    
2. 全局变量：声明时，在对应的变量声明语句前添加 `!global` 前缀即可。
  
    **局部变量和全局变量均拆成先定义后赋值**

3. 函数
    - 定义格式：`@func 函数名 (参数1, 参数2, ...) 函数体 @endfunc`。其中，函数名格式为 返回值类型_id（void 为 v）。参数可以是一个数据类型，也可以是一个指针。例如：`@func %d_9 (%i_10, %f_11, %ad_12, %b_14)` 表示 `double d_9(int a, float b, double *c, bool *d)`。这里指针对应原代码中的数组，我们把任何维度的数组都展平成一维数组处理。
    - 调用格式：
      - `void` ：`call void 函数名 ( 参数1 , 参数2 , ... )`
      - 非 `void`：`call 变量名 函数名 ( 参数1 , 参数2 , ... )`
    - 特殊函数：`print_int` 等库函数，在生成中间代码时直接 call，模拟器中按含义模拟即可；对于 `main` 函数，不会进行重命名，中间代码中的名字仍然为 `main`。

4. 指令
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

以下是 IR 代码的例子：
1. 
```
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

```
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
```
double foo(double x[2], double y[2]){
	return x[0] + y[0];
}

int main() {
	double a[2][2] = { {1.0, 2.0}, {4.5e-2} };
	foo( a[0], a[1] );
	return 0;
}
```

```
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
```
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

```
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
```
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

```
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
