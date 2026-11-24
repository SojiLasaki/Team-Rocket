#!/bin/bash
# Usage: ./run_tests.sh   (run from anywhere; writes test_results.txt next to this script)
cd "$(dirname "$0")" || exit 1
OUT=test_results.txt
TMP=$(mktemp -d)
g++ bin_to_hex.cpp -o "$TMP/b2h" && g++ hex_to_bin.cpp -o "$TMP/h2b" || { echo "Build failed"; exit 1; }

pass=0; fail=0
{
echo "Test run: $(date)"
echo "================================"
} > "$OUT"

# check <program> <input> <expected output|ERROR>
check() {
    local prog=$1 input=$2 expected=$3 actual code label
    actual=$(echo "$input" | "$TMP/$prog" 2>/dev/null); code=$?
    actual=${actual##*: }
    if [ "$expected" = "ERROR" ]; then
        [ $code -ne 0 ] && label=PASS || label=FAIL
        actual="(exit $code)"
    else
        [ "$actual" = "$expected" ] && label=PASS || label=FAIL
    fi
    [ $label = PASS ] && pass=$((pass+1)) || fail=$((fail+1))
    printf "%s  %-7s input=%-14s expected=%-14s got=%s\n" "$label" "$prog" "'$input'" "$expected" "$actual" >> "$OUT"
}

# Binary -> Hex
check b2h 0 0
check b2h 1 1
check b2h 1111 F
check b2h 10000 10
check b2h 11010110101 6B5
check b2h 0000 0
check b2h 1111111111111111 FFFF
check b2h 101010101010 AAA
check b2h 102 ERROR
check b2h abc ERROR

# Hex -> Binary
check h2b 0 0000
check h2b F 1111
check h2b 6B5 011010110101
check h2b 6b5 011010110101
check h2b 0x6B5 011010110101
check h2b FFFF 1111111111111111
check h2b A5 10100101
check h2b G1 ERROR
check h2b 0x ERROR

{
echo "================================"
echo "Passed: $pass  Failed: $fail"
} >> "$OUT"

rm -rf "$TMP"
cat "$OUT"
[ $fail -eq 0 ]
