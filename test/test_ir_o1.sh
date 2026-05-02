#!/usr/bin/env bash

set -euo pipefail

BASEDIR="$(cd "$(dirname "$0")" && pwd)"
SAMPLES_DIR="$BASEDIR/../test/samples_codegen_functional"
COMPILER="$BASEDIR/../build/compiler"
INTERP="$BASEDIR/../build/interpreter"

TMPDIR=$(mktemp -d)
trap 'rm -rf "$TMPDIR"' EXIT

FAILED=""

# Directory to keep generated IR files (persistent)
OUTDIR="$BASEDIR/ir_files"
mkdir -p "$OUTDIR"

shopt -s nullglob
files=("$SAMPLES_DIR"/*.cact)
if [ ${#files[@]} -eq 0 ]; then
	echo "No .cact files found in $SAMPLES_DIR"
	exit 1
fi

for file in "${files[@]}"; do
	echo -e "\033[;36mTesting $file...\033[0m"
	base=$(basename "$file" .cact)
	irfile="$OUTDIR/${base}.ir"
	compile_err="$TMPDIR/${base}.compile.err"

	# Compile to IR
	if ! "$COMPILER" "$file" "--emit-IR" "-O1" -o "$irfile" 2>"$compile_err"; then
		echo -e "\033[;31m[FAIL] Compiler failed for $file\033[0m"
		echo "--- Compiler stderr ---"
		sed -n '1,200p' "$compile_err" || true
		FAILED=$base
		break
	fi

	# Run interpreter with optional .in file
	actual_out="$TMPDIR/${base}.actual"
	interp_err="$TMPDIR/${base}.interp.err"
	input_file="$SAMPLES_DIR/${base}.in"
	if [ -f "$input_file" ]; then
		if ! "$INTERP" "$irfile" < "$input_file" > "$actual_out" 2>"$interp_err"; then
			echo -e "\033[;31m[FAIL] Interpreter failed for $base (with input)\033[0m"
			echo "--- Interpreter stderr ---"
			sed -n '1,200p' "$interp_err" || true
			FAILED=$base
			break
		fi
	else
		if ! "$INTERP" "$irfile" > "$actual_out" 2>"$interp_err"; then
			echo -e "\033[;31m[FAIL] Interpreter failed for $base\033[0m"
			echo "--- Interpreter stderr ---"
			sed -n '1,200p' "$interp_err" || true
			FAILED=$base
			break
		fi
	fi

	# Compare with expected .out if exists
	expected="$SAMPLES_DIR/${base}.out"
	if [ -f "$expected" ]; then
		# expected .out: first line is junk, real expected output starts from line 2
		expected_trim="$TMPDIR/${base}.expected_trim"
		tail -n +2 "$expected" > "$expected_trim"

		# Modify actual output: compute last line modulo 256 and replace last line
		actual_mod="$TMPDIR/${base}.actual_mod"
		last_line=$(tail -n 1 "$actual_out" 2>/dev/null || echo "")
				# compute modulo using python to support 0/0x prefixes
				# pass the last_line as an argument to avoid stdin/heredoc issues
				if ! mod_val=$(python3 -c '
import sys
s = sys.argv[1].strip()
try:
		v = int(s, 0)
		print((v % 256 + 256) % 256)
except Exception:
		sys.exit(2)
' "$last_line" 2>/dev/null); then
						mod_val=""
				fi

		if [ -z "$mod_val" ]; then
			echo -e "\033[;31m[FAIL] Cannot parse last line as integer for $base\033[0m"
			echo "--- Actual last line ---"
			printf '%s\n' "$last_line"
			FAILED=$base
			break
		fi

		# write all but last line, then append mod_val
		sed '$d' "$actual_out" > "$actual_mod" || true
		printf "%s\n" "$mod_val" >> "$actual_mod"

		if diff -u --strip-trailing-cr "$expected_trim" "$actual_mod" > "$TMPDIR/${base}.diff"; then
			echo -e "\033[;32m[PASS] $base\033[0m"
		else
			echo -e "\033[;31m[FAIL] Output differs for $base\033[0m"
			echo "--- diff ---"
			sed -n '1,200p' "$TMPDIR/${base}.diff" || true
			FAILED=$base
      break
		fi
	else
		echo -e "\033[;33m No expected .out file for $base; skipping comparison\033[0m"
		echo -e "Interpreter output saved to $actual_out"
	fi

done

if [ -n "$FAILED" ]; then
	echo -e "\033[;31mTest failed on $FAILED\033[0m"
	exit 1
else
	echo -e "\033[;32mAll tests passed!\033[0m"
	exit 0
fi