#!/bin/bash
#
# This script builds the ZeroPilot-SW Autopilot subproject
#
# This file is part of WARG's ZeroPilot-SW
#
# Copyright (c) 2018, Waterloo Aerial Robotics Group (WARG)
# All rights reserved.
#
# This software is licensed under a modified version of the BSD 3 clause license
# that should have been included with this software in a file called LICENSE.md
# Otherwise it is available at:
# https://raw.githubusercontent.com/UWARG/ZeroPilot-SW/devel/LICENSE.md
#

# Enable exit tracing and variable protection
set -o errtrace
set -o nounset

CLEAN=true
BUILD_TYPE="Debug"
GENERATOR="Unix Makefiles"
PLATFORM="nucleol552zeq"

while getopts c:p: flag
do
    case "${flag}" in
        c) CLEAN={OPTARG};;
        p) PLATFORM=${OPTARG};;
    esac
done

echo "Building for $PLATFORM"

if command -v ninja >/dev/null 2>&1; then
    GENERATOR="Ninja"
elif command -v make >/dev/null 2>&1; then
    GENERATOR="Unix Makefiles"
elif command -v mingw32-make >/dev/null 2>&1; then
    GENERATOR="MinGW Makefiles"
fi

die() {
    echo ""
    echo "LaminarOS build FAILED!"
    echo "Error $1 was encountered on line $2."
    exit $1
}

# Set up exit condition
trap 'die $? $LINENO' ERR

BUILD_DIR="build"

if [[ $CLEAN == true ]]; then
    echo "Cleaning old build environment"
    cmake -E remove_directory $BUILD_DIR
fi

# Prebuild info display
echo "Building ZeroPilot..."

# Build commands
cmake -E make_directory $BUILD_DIR
cmake -E chdir $BUILD_DIR \
  cmake \
    -G "${GENERATOR}" \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
    -DCMAKE_TOOLCHAIN_FILE="Boardfiles/$PLATFORM/$PLATFORM.cmake" \
    -Wdev\
    -Wdeprecated\
    ../

cmake --build $BUILD_DIR

# Final status display
echo ""
echo "ZeroPilot build SUCCESS!"
exit 0
