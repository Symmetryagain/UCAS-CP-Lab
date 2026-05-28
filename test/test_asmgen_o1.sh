#!/usr/bin/env bash

set -euo pipefail

BASEDIR="$(cd "$(dirname "$0")" && pwd)"
SAMPLES_DIR="$BASEDIR/samples_codegen_functional"
COMPILER="$BASEDIR/../build/compiler"

RISCV_GCC="/opt/riscv64/bin/riscv64-unknown-elf-gcc"
SPIKE="/opt/spike/bin/spike"

LIB_DIR="$BASEDIR/../lib"

TMPDIR=$(mktemp -d)
trap 'rm -rf "$TMPDIR"' EXIT

FAILED=""

IRDIR="$BASEDIR/ir_files"
ASMDIR="$BASEDIR/asm_files_o1"
ELFDIR="$BASEDIR/elf_files_o1"
mkdir -p "$IRDIR" "$ASMDIR" "$ELFDIR"

shopt -s nullglob
files=("$SAMPLES_DIR"/*.cact)
if [ ${#files[@]} -eq 0 ]; then
    echo "No .cact files found in $SAMPLES_DIR"
    exit 1
fi

command -v "$RISCV_GCC" >/dev/null 2>&1 || { echo "Error: $RISCV_GCC not found"; exit 1; }
command -v "$SPIKE" >/dev/null 2>&1 || { echo "Error: $SPIKE not found"; exit 1; }

total=${#files[@]}
current=1
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

    # 1. Compile CACT to RISC-V assembly with -O1
    echo "  1. Compiling to assembly (-O1)..."
    if ! "$COMPILER" "$file" -o "$asmfile" -O1 2>"$compile_err"; then
        echo -e "\033[;31m[FAIL] Compiler failed for $file\033[0m"
        sed -n '1,20p' "$compile_err" || true
        FAILED=$base
        failed_count=$((failed_count + 1))
        continue
    fi
    echo -e "   \033[;32m✓ Assembly: $asmfile\033[0m"

    # Also generate IR for reference
    "$COMPILER" "$file" --emit-IR -o "$irfile" -O1 2>/dev/null || true

    # 2. Assemble and link to RISC-V ELF
    echo "  2. Linking to ELF..."
    if ! "$RISCV_GCC" "$asmfile" -L"$LIB_DIR" -lcactio -o "$elffile" 2>"$TMPDIR/${base}.gcc.err"; then
        echo -e "\033[;31m[FAIL] Link failed for $base\033[0m"
        sed -n '1,20p' "$TMPDIR/${base}.gcc.err" || true
        FAILED=$base
        failed_count=$((failed_count + 1))
        continue
    fi
    echo -e "   \033[;32m✓ ELF: $elffile\033[0m"

    # Check for input file
    input_file="$SAMPLES_DIR/${base}.in"
    if [ -f "$input_file" ]; then
        echo -e "   \033[;33m⚠ Skipping spike (requires input)\033[0m"
        manual_test_files+=("$base")
        skipped_count=$((skipped_count + 1))
        echo -e "\033[;33m[SKIP] $base\033[0m"
        echo ""
        continue
    fi

    # 3. Spike execution (if available)
    if "$SPIKE" --help >/dev/null 2>&1; then
        echo "  3. Running on spike..."
        actual_out="$TMPDIR/${base}.actual"
        spike_err="$TMPDIR/${base}.spike.err"

        set +e
        ("$SPIKE" pk "$elffile" > "$actual_out" 2>"$spike_err")
        spike_exit_code=$?
        set -e

        if [ $spike_exit_code -eq 255 ] && grep -q "could not open pk" "$spike_err" 2>/dev/null; then
            echo -e "   \033[;33m⚠ Spike pk not found — skipping execution tests\033[0m"
            echo -e "\033[;33m[COMPILE-ONLY] $base\033[0m"
            passed_count=$((passed_count + 1))
            echo ""
            continue
        fi

        mod_val=$(( (spike_exit_code % 256 + 256) % 256 ))
        printf "%s\n" "$mod_val" >> "$actual_out"
        echo -e "   \033[;32m✓ Spike done (exit=$spike_exit_code mod256=$mod_val)\033[0m"

        [ -s "$spike_err" ] && echo -e "   \033[;33m⚠ Spike stderr:\033[0m $(head -3 "$spike_err")"

        # 4. Compare output
        expected="$SAMPLES_DIR/${base}.out"
        if [ -f "$expected" ]; then
            expected_trim="$TMPDIR/${base}.expected_trim"
            tail -n +2 "$expected" > "$expected_trim"

            if diff -u --strip-trailing-cr "$expected_trim" "$actual_out" > "$TMPDIR/${base}.diff"; then
                echo -e "   \033[;32m✓ Match\033[0m"
                echo -e "\033[;32m[PASS] $base\033[0m"
            else
                echo -e "\033[;31m[FAIL] Mismatch for $base\033[0m"
                head -20 "$TMPDIR/${base}.diff" || true
                FAILED=$base
                failed_count=$((failed_count + 1))
                echo ""
                continue
            fi
        else
            echo -e "   \033[;33mNo .out for $base\033[0m"
        fi
        passed_count=$((passed_count + 1))
    else
        echo -e "   \033[;33mSpike not available — compile+link only\033[0m"
        echo -e "\033[;33m[COMPILE-ONLY] $base\033[0m"
        passed_count=$((passed_count + 1))
    fi
    echo ""
done

echo -e "\033[;34m═══════════════════════════════════════════════════\033[0m"
echo -e "\033[;34mTest summary:\033[0m"
echo -e "\033[;32m  Passed: $passed_count\033[0m"
echo -e "\033[;31m  Failed: $failed_count\033[0m"
echo -e "\033[;33m  Skipped: $skipped_count\033[0m"
echo -e "\033[;34m  Total:  $total\033[0m"
echo -e "\033[;34m═══════════════════════════════════════════════════\033[0m"

if [ ${#manual_test_files[@]} -gt 0 ]; then
    echo -e "\033[;33mFiles needing manual spike test (have .in input):\033[0m"
    for t in "${manual_test_files[@]}"; do
        echo -e "  $t: $SPIKE pk $ELFDIR/$t.elf < $SAMPLES_DIR/$t.in"
    done
fi

echo ""
echo -e "\033[;33mGenerated files:\033[0m"
echo -e "  IR:  $IRDIR/"
echo -e "  ASM: $ASMDIR/"
echo -e "  ELF: $ELFDIR/"

[ $failed_count -gt 0 ] && exit 1
exit 0
