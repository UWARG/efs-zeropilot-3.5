#!/usr/bin/env bash

# define path to the tools dir
TOOLS_DIR=$(dirname -- "$0")/..

SRC_FILES=$(bash $TOOLS_DIR/scripts/static_analysis_files.bash)
mkdir -p $TOOLS_DIR/lint_output
cppcheck $SRC_FILES 2>&1 | tee $TOOLS_DIR/lint_output/cppcheck.txt