#!/usr/bin/env bash

# define path to this bash script
SCRIPT_PATH=$(dirname -- "$0")

SRC_FILES=$(bash $SCRIPT_PATH/static_analysis_files.bash)
mkdir -p $SCRIPT_PATH/LintOutput/formatted-files
clang-format -i $SRC_FILES
cp $SRC_FILES $SCRIPT_PATH/LintOutput/formatted-files/