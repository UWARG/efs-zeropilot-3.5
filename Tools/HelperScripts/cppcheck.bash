#!/usr/bin/env bash

# define path to the tools dir
TOOLS_DIR=$(dirname -- "$0")/..

SRC_FILES=$(bash $TOOLS_DIR/static_analysis_files.bash)
mkdir -p $TOOLS_DIR/LintOutput
cppcheck $SRC_FILES 2>&1 | tee $TOOLS_DIR/LintOutput/cppcheck.txt