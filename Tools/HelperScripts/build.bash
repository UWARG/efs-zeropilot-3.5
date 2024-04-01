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
done < "$TOOLS_DIR/default_config.txt"

# read user inputs
CLEAN=false   
while getopts :t:p:m:c opt
do
    case ${opt} in
        t) 
            COMPILE_TYPE=${OPTARG}
            ;;
        p) 
            PLATFORM=${OPTARG}
            ;;
        m) 
            MODEL_NAME=${OPTARG}
            ;;
        c) 
        CLEAN=true
        ;;
        \?) 
            throw_error "Error: Flag -$OPTARG is undefined for compile."
            ;;
    esac
done

# ====================
# ------compile-------
# ====================

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
COMPILE_DIR="$TOOLS_DIR/$COMPILE_TYPE/build" 
if [[ $COMPILE_TYPE == "Firmware" ]]; then
    echo "Building for platform $PLATFORM and model $MODEL_NAME."
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
            -DCMAKE_TOOLCHAIN_FILE="$TOOLS_DIR/../Boardfiles/$PLATFORM/$PLATFORM.cmake" \
            -DMODEL_NAME="$MODEL_NAME" \
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
