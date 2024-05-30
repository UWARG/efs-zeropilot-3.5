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

mkdir -p $TOOLS_DIR/lint_output/formatted_files

# Get differences between the original and formatted files using a for loop
for file in $SRC_FILES; do
    clang-format $CLANG_FORMAT_FLAGS $file > $TOOLS_DIR/lint_output/formatted_files/$(basename $file)

    # if the file exists in the formatted_files
    if [ -f $TOOLS_DIR/lint_output/formatted_files/$(basename $file) ]; then
        diff -u $file $TOOLS_DIR/lint_output/formatted_files/$(basename $file)
    fi
done
