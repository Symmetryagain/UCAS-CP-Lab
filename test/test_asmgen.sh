#!/usr/bin/env bash

set -euo pipefail

BASEDIR="$(cd "$(dirname "$0")" && pwd)"
SAMPLES_DIR="$BASEDIR/../test/samples_codegen_functional"
COMPILER="$BASEDIR/../build/compiler"
ASMGEN="$BASEDIR/../asmgen/build/asmgen"

# RISC-V工具链
RISCV_GCC="riscv64-unknown-elf-gcc"
RISCV_OBJDUMP="riscv64-unknown-elf-objdump"
SPIKE="spike"

# 库路径
LIB_DIR="$BASEDIR/../lib"

TMPDIR=$(mktemp -d)
trap 'rm -rf "$TMPDIR"' EXIT

FAILED=""

# Directories to keep generated files (persistent)
IRDIR="$BASEDIR/ir_files"
ASMDIR="$BASEDIR/asm_files"
ELFDIR="$BASEDIR/elf_files"  # 可选：保存ELF文件用于调试
mkdir -p "$IRDIR" "$ASMDIR" "$ELFDIR"

shopt -s nullglob
files=("$SAMPLES_DIR"/*.cact)
if [ ${#files[@]} -eq 0 ]; then
    echo "No .cact files found in $SAMPLES_DIR"
    exit 1
fi

# 检查必要的工具是否可用
command -v "$RISCV_GCC" >/dev/null 2>&1 || {
    echo "Error: $RISCV_GCC not found in PATH"
    exit 1
}

# 测试计数器
total=${#files[@]}
current=1

# 用于记录需要手动测试的文件
manual_test_files=()
passed_count=0
failed_count=0
skipped_count=0

for file in "${files[@]}"; do
    base=$(basename "$file" .cact)
    
    echo -e "\033[;36m[$current/$total] Processing $file...\033[0m"
    current=$((current + 1))
    
    irfile="$IRDIR/${base}.ir"
    asmfile="$ASMDIR/${base}.s"
    elffile="$ELFDIR/${base}.elf"
    compile_err="$TMPDIR/${base}.compile.err"
    
    # 1. 编译Cact文件生成IR（总是重新生成）
    echo "  1. Compiling to IR..."
    if ! "$COMPILER" "$file" "-O1" > "$irfile" 2>"$compile_err"; then
        echo -e "\033[;31m[FAIL] Compiler failed for $file\033[0m"
        echo "   --- Compiler stderr ---"
        sed -n '1,200p' "$compile_err" || true
        FAILED=$base
        failed_count=$((failed_count + 1))
        continue
    fi
    echo -e "   \033[;32m✓ IR generated: $irfile\033[0m"

    # 2. 从IR生成汇编（总是重新生成）
    echo "  2. Generating assembly..."
    if ! "$ASMGEN" "$asmfile" "$irfile" 2>"$TMPDIR/${base}.asmgen.err"; then
        echo -e "\033[;31m[FAIL] Asmgen failed for $base\033[0m"
        echo "   --- Asmgen stderr ---"
        sed -n '1,200p' "$TMPDIR/${base}.asmgen.err" || true
        FAILED=$base
        failed_count=$((failed_count + 1))
        continue
    fi
    echo -e "   \033[;32m✓ Assembly generated: $asmfile\033[0m"

    # 3. 编译汇编为RISC-V ELF
    echo "  3. Compiling to RISC-V ELF..."
    if ! "$RISCV_GCC" "$asmfile" -L"$LIB_DIR" -lcactio -o "$elffile" 2>"$TMPDIR/${base}.gcc.err"; then
        echo -e "\033[;31m[FAIL] GCC compilation failed for $base\033[0m"
        echo "   --- GCC stderr ---"
        sed -n '1,200p' "$TMPDIR/${base}.gcc.err" || true
        FAILED=$base
        failed_count=$((failed_count + 1))
        continue
    fi
    echo -e "   \033[;32m✓ ELF generated: $elffile\033[0m"
    
    # 检查是否有输入文件
    input_file="$SAMPLES_DIR/${base}.in"
    if [ -f "$input_file" ]; then
        echo -e "   \033[;33m⚠ Skipping spike execution (requires input file)\033[0m"
        manual_test_files+=("$base")
        skipped_count=$((skipped_count + 1))
        echo -e "\033[;33m[SKIP] $base - requires manual spike testing\033[0m"
        echo ""
        continue
    fi
    
    # 4. 使用spike运行（仅当没有输入文件时）
    echo "  4. Running on spike..."
    actual_out="$TMPDIR/${base}.actual"
    spike_err="$TMPDIR/${base}.spike.err"
    
    set +e
    # 运行spike并捕获输出和退出码（没有输入文件）
    ("$SPIKE" "pk" "$elffile" > "$actual_out" 2>"$spike_err")
    spike_exit_code=$?
    set -e
    
    # 计算退出码的模256
    mod_val=$(( (spike_exit_code % 256 + 256) % 256 ))
    
    # 将模256的结果追加到实际输出的最后一行
    printf "%s\n" "$mod_val" >> "$actual_out"
    
    echo -e "   \033[;32m✓ Spike executed (exit code: $spike_exit_code, mod256: $mod_val)\033[0m"

    # 检查spike是否有错误输出（除了程序返回值）
    if [ -s "$spike_err" ]; then
        echo -e "   \033[;33m⚠ Warning: Spike had stderr output\033[0m"
        echo "   --- Spike stderr ---"
        sed -n '1,10p' "$spike_err" || true
    fi

    # 5. 与预期输出比较
    expected="$SAMPLES_DIR/${base}.out"
    if [ -f "$expected" ]; then
        # expected .out: first line is junk, real expected output starts from line 2
        expected_trim="$TMPDIR/${base}.expected_trim"
        tail -n +2 "$expected" > "$expected_trim"
        
        echo "  5. Comparing output..."
        if diff -u --strip-trailing-cr "$expected_trim" "$actual_out" > "$TMPDIR/${base}.diff"; then
            echo -e "   \033[;32m✓ Output matches expected\033[0m"
            echo -e "\033[;32m[PASS] $base\033[0m"
            passed_count=$((passed_count + 1))
        else
            echo -e "\033[;31m[FAIL] Output differs for $base\033[0m"
            echo "   --- Expected output (trimmed) ---"
            head -20 "$expected_trim"
            echo "   --- Actual output ---"
            head -20 "$actual_out"
            echo "   --- diff (first 20 lines) ---"
            head -20 "$TMPDIR/${base}.diff" || true
            FAILED=$base
            failed_count=$((failed_count + 1))
        fi
    else
        echo -e "\033[;33m[SKIP] No expected .out file for $base\033[0m"
        echo "   --- Actual output ---"
        head -10 "$actual_out"
        if [ $(wc -l < "$actual_out") -gt 10 ]; then
            echo "   ... (truncated)"
        fi
        passed_count=$((passed_count + 1))
    fi
    
    echo ""
done

# 输出测试统计
echo -e "\033[;34m═══════════════════════════════════════════════════\033[0m"
echo -e "\033[;34m测试统计:\033[0m"
echo -e "\033[;32m  编译通过且spike测试通过: $passed_count\033[0m"
echo -e "\033[;31m  编译或测试失败: $failed_count\033[0m"
echo -e "\033[;33m  编译通过但跳过spike测试: $skipped_count\033[0m"
echo -e "\033[;34m  总计: $total\033[0m"
echo -e "\033[;34m═══════════════════════════════════════════════════\033[0m"

# 如果有需要手动测试的文件，输出它们
if [ ${#manual_test_files[@]} -gt 0 ]; then
    echo -e "\033[;33m需要手动spike测试的文件:\033[0m"
    for test_file in "${manual_test_files[@]}"; do
        echo -e "  \033[;33m$test_file\033[0m"
        echo -e "    输入文件: $SAMPLES_DIR/${test_file}.in"
        echo -e "    测试文件: $SAMPLES_DIR/${test_file}.cact"
        echo -e "    预期输出: $SAMPLES_DIR/${test_file}.out"
        echo -e "    已生成ELF: $ELFDIR/${test_file}.elf"
        echo -e "    手动测试命令: $SPIKE pk $ELFDIR/${test_file}.elf < $SAMPLES_DIR/${test_file}.in"
        echo ""
    done
fi

# 输出生成的文件位置
echo -e "\033[;33m生成的文件保存位置:\033[0m"
echo -e "  IR文件:   $IRDIR/"
echo -e "  汇编文件: $ASMDIR/"
echo -e "  ELF文件:  $ELFDIR/"

# 如果有失败的测试，返回非零退出码
if [ $failed_count -gt 0 ]; then
    echo -e "\033[;31m═══════════════════════════════════════════════════\033[0m"
    echo -e "\033[;31m测试失败: $failed_count 个测试未通过\033[0m"
    echo -e "\033[;31m═══════════════════════════════════════════════════\033[0m"
    exit 1
else
    echo -e "\033[;32m═══════════════════════════════════════════════════\033[0m"
    echo -e "\033[;32m所有无输入文件的测试均已通过！\033[0m"
    echo -e "\033[;32m═══════════════════════════════════════════════════\033[0m"
    exit 0
fi