# define path to this bash script
SCRIPT_PATH=$(dirname -- "$0")

# grab source file paths from compile_commands.json (excluding Boardfiles)
SRC_FILES=($(grep -e "\"file\": \"/src/.*\.cp*" $SCRIPT_PATH/Firmware/build/compile_commands.json | grep -v "Boardfiles"))
SRC_FILES+=($(grep -e "\"file\":.*main.cpp" $SCRIPT_PATH/Firmware/build/compile_commands.json))
# filter none file path strings
SRC_FILES=($(printf "%s\n" "${SRC_FILES[@]}" | grep -v 'file'))
# trim quotation marks in file path string
for i in "${!SRC_FILES[@]}"; do
    SRC_FILES[$i]=${SRC_FILES[$i]:1:-1}
done

COMPILER_ROOT=/usr/share/arm-gnu-toolchain-12.3.rel1-x86_64-arm-none-eabi
for SRC_FILE in "${SRC_FILES[@]}"
do
    echo "clang-tidy ${SRC_FILE}"
    clang-tidy --checks=-*,cppcoreguidelines-avoid-do-while \
        -p /src/Tools/Firmware/build \
        --extra-arg=--target=arm-none-eabi \
        --extra-arg=-I"${COMPILER_ROOT}/arm-none-eabi/include/c++/12.3.1" \
        --extra-arg=-I"${COMPILER_ROOT}/arm-none-eabi/include/c++/12.3.1/arm-none-eabi" \
        --extra-arg=-I"${COMPILER_ROOT}/arm-none-eabi/include/c++/12.3.1/backward" \
        --extra-arg=-I"${COMPILER_ROOT}/lib/gcc/arm-none-eabi/12.3.1/include" \
        --extra-arg=-I"${COMPILER_ROOT}/lib/gcc/arm-none-eabi/12.3.1/include-fixed" \
        --extra-arg=-I"${COMPILER_ROOT}/arm-none-eabi/include" \
        $SRC_FILE
done