#!/usr/bin/env bash

# define path to this bash script
SCRIPT_PATH=$(dirname -- "$0")

SRC_FILES=$(bash $SCRIPT_PATH/static_analysis_files.bash)
mkdir -p $SCRIPT_PATH/LintOutput
cppcheck $SRC_FILES 2>&1 | tee $SCRIPT_PATH/LintOutput/cppcheck.txt