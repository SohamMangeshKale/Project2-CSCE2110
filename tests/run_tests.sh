#!/bin/bash
# File:    tests/run_tests.sh
# Purpose: Runs every Milestone 1 test (member_a_tests.sh + member_b_tests.sh) against the
#          compiled program and prints a results table.
# Author:  Member A and Member B (shared)
#
# Usage (from the Project2 folder, after compiling to ./campus):
#     bash tests/run_tests.sh > test_results_run.txt
#     bash tests/run_tests.sh ./campus        (explicit executable)
# Every "Actual" line is real output captured from the program during this run.

EXE=${1:-./campus}
PASS=0
FAIL=0
source tests/test_lib.sh

echo "MILESTONE 1 TEST RUN"
echo "Executable: $EXE"
echo "Run date:   $(date)"
echo "Compiler:   $(g++ --version | head -1)"
echo "Machine:    $(uname -a)"
echo "======================================================================"
echo
echo "---- Member A tests (loading helpers, facilities, hash table, menu) ----"
echo
source tests/member_a_tests.sh
echo "---- Member B tests (graph, BFS, department tree, graph/tree loading) ----"
echo
source tests/member_b_tests.sh

echo "======================================================================"
echo "SUMMARY: $PASS passed, $FAIL failed, $((PASS+FAIL)) total"
[ "$FAIL" -eq 0 ]
