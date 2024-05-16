#!/usr/bin/env bash

# define path to the tools dir
TOOLS_DIR=$(dirname -- "$0")/..

# read lint configurations
source $TOOLS_DIR/scripts/read_configs.bash "$TOOLS_DIR/configs/lint_config.txt"

# get source files
SRC_FILES=$(bash $TOOLS_DIR/scripts/static_analysis_files.bash)
if [ -z "$SRC_FILES" ]; then
    >&2 echo "No source files found for linting."
    exit
fi

mkdir -p $TOOLS_DIR/lint_output
cppcheck $CPPCHECK_FLAGS $SRC_FILES 2>&1 | tee $TOOLS_DIR/lint_output/cppcheck.txt