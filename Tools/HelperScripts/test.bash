#!/usr/bin/env bash


# ====================
# --- script setup ---
# ====================

# exit on attempt to use undeclared variable
set -o nounset
# enable error tracing
set -o errtrace
# define path to the tools dir
TOOLS_DIR=$(dirname -- "$0")/..

# display error message
throw_error() {
    echo ""
    echo $1
    exit 1
}


# ====================
# --- var declare ---
# ====================

# read user inputs
while getopts :f: opt
do
    case "${opt}" in
        f) 
            TEST_FILTER=${OPTARG}
            ;;
        \?) 
            throw_error "Error: Flag -$OPTARG is undefined for run."
            ;;
    esac
done

# ====================
# -----run tests------
# ====================

TEST_PROGRAM="$TOOLS_DIR/Testing/build/unit_testing"

# check for test executable existance
if [[ ! -f $TEST_PROGRAM  ]]; then
    throw_error "Error: No test executable found. Build testing project first."
fi

$TEST_PROGRAM --gtest_filter=$TEST_FILTER
exit $?
