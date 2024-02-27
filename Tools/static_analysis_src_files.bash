#!/usr/bin/env bash

# define path to this bash script
SCRIPT_PATH=$(dirname -- "$0")

# exclude paths specified in static_analysis_ignore.txt
PRUNE_PATHS="\( "
while IFS=$'\n' read -r line; do
    if [[ $line != *#* && $line != "" ]]; then
        PRUNE_PATHS+="-path /src/${line} -o "
    fi
done < "$SCRIPT_PATH/static_analysis_ignore.txt"
PRUNE_PATHS=${PRUNE_PATHS:0:-3}
PRUNE_PATHS+="\)"
# construct search command
FIND_CMD="find /src $PRUNE_PATHS -prune -o -print"
GREP_CMD="grep \"\.\(c\|h\)\(pp\)*$\" | grep -v Test" # exclude test files
SEARCH_CMD="$FIND_CMD | $GREP_CMD"

# search for source and header files
SRC_FILES=($(eval $SEARCH_CMD))
# add main.cpp
SRC_FILES+=($(find /src -name "*main.cpp"))

echo "${SRC_FILES[@]}"