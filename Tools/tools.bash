#!/usr/bin/env bash


# ====================
# --- script setup ---
# ====================

# exit on attempt to use undeclared variable
set -o nounset
# enable error tracing
set -o errtrace
# define path to this bash script
SCRIPT_PATH=$(dirname -- "$0")

# display error message
throw_error() {
    echo ""
    echo $1
    exit 1
}


# ====================
# --- var declare ---
# ====================

# read default configurations
while read -r line
do
    # skip comment lines and blank lines
    if [[ $line =~ ^\s*#.*$ ]] || [[ -z $(echo $line | tr -d '[:space:]') ]]; then
        continue
    fi

    # split line at "=" into a key-value pair
    IFS="=" read -r key value <<< "$line"
    
    # strip extra spaces
    key=$(echo $key)
    value=$(echo $value)

    # declare variables
    declare $key="${value//\"/}"
done < "$SCRIPT_PATH/default_config.txt"

# read user inputs
# missing function
if [[ $# -eq 0 ]]; then
    throw_error "Error: No function supplied."
# compile settings
elif [[ $1 == "compile" ]]; then
    RUN_TEST=false
    CLEAN=false   
    shift 1 && while getopts :t:p:c opt
    do
        case ${opt} in
            t) 
                COMPILE_TYPE=${OPTARG}
                ;;
            p) 
                PLATFORM=${OPTARG}
                ;;
            c) 
            CLEAN=true
            ;;
            \?) 
                throw_error "Error: Flag -$OPTARG is undefined for compile."
                ;;
        esac
    done
# run setting
elif [[ $1 == "run" ]]; then
    RUN_TEST=true     
    shift 1 && while getopts :f: opt
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
# bad function
else
    throw_error "Error: Function $1 is undefined."
fi


# ====================
# --- compile/run---
# ====================

# compile project
if [[ $RUN_TEST == false ]]; then 
    # validate build type
    if [[ $COMPILE_TYPE != "Firmware" ]] && [[ $COMPILE_TYPE != "Testing" ]]; then
        throw_error "Error: Build type $COMPILE_TYPE is undefined."
    fi

    # select generator
    if command -v ninja >/dev/null 2>&1; then
        GENERATOR="Ninja"
    elif command -v make >/dev/null 2>&1; then
        GENERATOR="Unix Makefiles"
    elif command -v mingw32-make >/dev/null 2>&1; then
        GENERATOR="MinGW Makefiles"
    else
        throw_error "Error: No cmake generator found."
    fi

    # compile setup
    echo "Building ZeroPilot for $(echo $COMPILE_TYPE | tr '[:upper:]' '[:lower:]')."  
    COMPILE_DIR="$SCRIPT_PATH/$COMPILE_TYPE/build" 
    if [[ $COMPILE_TYPE == "Firmware" ]]; then
        echo "Building for $PLATFORM."
    fi 
    if [[ $CLEAN == true ]]; then
        echo "Cleaning old $(echo $COMPILE_TYPE | tr '[:upper:]' '[:lower:]') build environment."
        cmake -E rm -rf $COMPILE_DIR
    fi

    # create build system
    echo ""
    echo "Creating $GENERATOR build system..."
    cmake -E make_directory $COMPILE_DIR
    if [[ $COMPILE_TYPE == "Firmware" ]]; then
        cmake -E chdir $COMPILE_DIR \
            cmake \
                -G "${GENERATOR}" \
                -DCMAKE_BUILD_TYPE="Debug" \
                -DCMAKE_TOOLCHAIN_FILE="../../../Boardfiles/$PLATFORM/$PLATFORM.cmake" \
                -Wdev \
                -Wdeprecated \
                ../
    else
        cmake -E chdir $COMPILE_DIR \
            cmake \
                -G "${GENERATOR}" \
                ../
    fi
    if [[ ! $? -eq 0 ]]; then
        throw_error "Error: Failed to create $GENERATOR build system."
    fi
    
    # compile project
    echo ""
    echo "Compiling project..."
    cmake --build $COMPILE_DIR
    if [[ ! $? -eq 0 ]]; then
        throw_error "Error: Failed to compile project."
    fi

    # compile done
    echo ""
    echo "ZeroPilot $(echo $COMPILE_TYPE | tr '[:upper:]' '[:lower:]') build SUCCESS!"
    exit 0
# run tests
else 
    TEST_PROGRAM="$SCRIPT_PATH/Testing/build/unit_testing"
    
    # check for test executable existance
    if [[ ! -f $TEST_PROGRAM  ]]; then
        throw_error "Error: No test executable found. Build testing project first."
    fi
    
    $TEST_PROGRAM --gtest_filter=$TEST_FILTER
    exit $?
fi
