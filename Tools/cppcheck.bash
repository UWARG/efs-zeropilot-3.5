#!/usr/bin/env bash

# define path to this bash script
SCRIPT_PATH=$(dirname -- "$0")

SRC_FILES=$(bash $SCRIPT_PATH/static_analysis_files.bash)
cppcheck $SRC_FILES