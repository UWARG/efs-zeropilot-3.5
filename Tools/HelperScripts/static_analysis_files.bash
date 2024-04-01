#!/usr/bin/env bash

# define path to the tools dir
TOOLS_DIR=$(dirname -- "$0")/..
JSON_PATH=$TOOLS_DIR/Firmware/build/compile_commands.json

# exclude paths specified in static_analysis_ignore.txt
EXCLUDE=''
while IFS=$'\n' read -r line; do
    if [[ $line != *#* && $line != "" ]]; then
        EXCLUDE+="$line|"
    fi
done < "$TOOLS_DIR/static_analysis_ignore.txt"
EXCLUDE=${EXCLUDE:0:-1}

ALL_INC_FILES=$(find /src -name "*.h*" | grep -Ev "$EXCLUDE")
TRAVERSE_STACK=()
FILES=()

# initialize traverse stack with source files
while read -r file; do
    TRAVERSE_STACK+=( $file )
done <<< $(grep -Po "\-c \/src\/.*\.cp*" $JSON_PATH | grep -Ev "$EXCLUDE" | sed "s/\-c //g" && \
           grep -Po "\-c \/src\/.*main\.cp*" $JSON_PATH | sed "s/\-c //g")

# traverse
while [ ${#TRAVERSE_STACK[@]} != 0 ]; do
    # pop from traverse stack
    file=${TRAVERSE_STACK[-1]}
    unset 'TRAVERSE_STACK[-1]'

    # skip if file is already in list
    if [[ "${FILES[@]}" == *$file* ]]; then
        continue
    fi

    # add file to list
    FILES+=( $file )

    # find all included headers in file
    while read -r header; do
        if [[ $header != "" && $ALL_INC_FILES == *$header* ]]; then
            # get full header path
            header_path=$(echo $ALL_INC_FILES | grep -Po "\/src[A-Za-z0-9_/]*\/$header")
            # add to traverse stack
            TRAVERSE_STACK+=( $header_path )
        fi
    done <<< $(cat $file | grep -Po "[A-Za-z0-9_]*\.hp*")
done

echo ${FILES[@]}