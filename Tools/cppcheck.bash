#!/usr/bin/env bash

# define path to this bash script
SCRIPT_PATH=$(dirname -- "$0")

# grab source file paths from compile_commands.json
SRC_FILES=($(grep -e "\"file\": \"/src/.*\.cp*" $SCRIPT_PATH/Firmware/build/compile_commands.json))
SRC_FILES+=($(grep -e "\"file\":.*main.cpp" $SCRIPT_PATH/Firmware/build/compile_commands.json))
# filter none file path strings
SRC_FILES=($(printf "%s\n" "${SRC_FILES[@]}" | grep -v '"file"'))
# exclude file/folder paths specified in clang-tidy_ignore.txt
while IFS=$'\n' read -r line; do
    if [[ $line != *#* && $line != "" ]]; then
        SRC_FILES=($(printf "%s\n" "${SRC_FILES[@]}" | grep -v "${line}"))
    fi
done < "$SCRIPT_PATH/clang-tidy_ignore.txt"
# trim quotation marks in file path string
for i in "${!SRC_FILES[@]}"; do
    SRC_FILES[$i]=${SRC_FILES[$i]:1:-1}
done


for SRC_FILE in "${SRC_FILES[@]}"
do
    cppcheck $SRC_FILE
done