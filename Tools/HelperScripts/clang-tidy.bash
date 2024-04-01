#!/usr/bin/env bash

# define path to the tools dir
TOOLS_DIR=$(dirname -- "$0")/..

SRC_FILES=$(bash $TOOLS_DIR/static_analysis_files.bash)
COMPILER_ROOT=/usr/share/arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi
mkdir -p $TOOLS_DIR/LintOutput
clang-tidy \
    -p /src/Tools/Firmware/build \
    --extra-arg=--target=arm-none-eabi \
    --extra-arg=-I"${COMPILER_ROOT}/arm-none-eabi/include/c++/12.3.1" \
    --extra-arg=-I"${COMPILER_ROOT}/arm-none-eabi/include/c++/12.3.1/arm-none-eabi" \
    --extra-arg=-I"${COMPILER_ROOT}/arm-none-eabi/include/c++/12.3.1/backward" \
    --extra-arg=-I"${COMPILER_ROOT}/lib/gcc/arm-none-eabi/12.3.1/include" \
    --extra-arg=-I"${COMPILER_ROOT}/lib/gcc/arm-none-eabi/12.3.1/include-fixed" \
    --extra-arg=-I"${COMPILER_ROOT}/arm-none-eabi/include" \
    $SRC_FILES \
    2>&1 | tee $TOOLS_DIR/LintOutput/clang-tidy.txt