#!/usr/bin/env bash

# define path to the tools dir
TOOLS_DIR=$(dirname -- "$0")/..

SRC_FILES=$(bash $TOOLS_DIR/scripts/static_analysis_files.bash)
mkdir -p $TOOLS_DIR/lint_output/formatted_files
clang-format -i $SRC_FILES
cp $SRC_FILES $TOOLS_DIR/lint_output/formatted_files/