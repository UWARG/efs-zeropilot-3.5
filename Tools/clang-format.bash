#!/usr/bin/env bash

# define path to this bash script
SCRIPT_PATH=$(dirname -- "$0")

SRC_FILES=$(bash $SCRIPT_PATH/static_analysis_src_files.bash)
clang-format -i $SRC_FILES