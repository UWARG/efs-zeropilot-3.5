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

COMPILER_ROOT=/usr/share/arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi
mkdir -p $TOOLS_DIR/lint_output
clang-tidy \
    -p $TOOLS_DIR/firmware/build \
    --extra-arg=--target=arm-none-eabi \
    --extra-arg=-I"${COMPILER_ROOT}/arm-none-eabi/include/c++/12.3.1" \
    --extra-arg=-I"${COMPILER_ROOT}/arm-none-eabi/include/c++/12.3.1/arm-none-eabi" \
    --extra-arg=-I"${COMPILER_ROOT}/arm-none-eabi/include/c++/12.3.1/backward" \
    --extra-arg=-I"${COMPILER_ROOT}/lib/gcc/arm-none-eabi/12.3.1/include" \
    --extra-arg=-I"${COMPILER_ROOT}/lib/gcc/arm-none-eabi/12.3.1/include-fixed" \
    --extra-arg=-I"${COMPILER_ROOT}/arm-none-eabi/include" \
    $CLANG_TIDY_FLAGS \
    $SRC_FILES \
    2>&1 | tee $TOOLS_DIR/lint_output/clang-tidy.txt