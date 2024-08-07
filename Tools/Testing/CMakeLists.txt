cmake_minimum_required(VERSION 3.18)
project(unit_testing)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define TESTING Preprocessor Macro
add_compile_definitions(TESTING)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")

# --- ZeroPilot File Indexing Begin ---

# Attitude Manager
set(AM_FOLDER "${ROOT_DIR}/AttitudeManager")
set(AM_INC "${AM_FOLDER}/Inc")
set(AM_SRC "${AM_FOLDER}/Src")
set(FLIGHTMODES_INC "${AM_FOLDER}/FlightModes/Inc")
set(FLIGHTMODES_SRC "${AM_FOLDER}/FlightModes/Src")

include_directories(${AM_INC} ${FLIGHTMODES_INC})
file(GLOB AM_CODE
    "${AM_FOLDER}/Tests/*.cpp"

    # add necessary AttitudeManager source files below
    "${AM_SRC}/*.cpp"
    "${FLIGHTMODES_SRC}/*.cpp"
)

# Path Manager
set(PM_FOLDER "${ROOT_DIR}/PathManager")
set(PM_INC "${PM_FOLDER}/Inc")
set(PM_SRC "${PM_FOLDER}/Src")

include_directories(${PM_INC})
file(GLOB PM_CODE
    "${PM_FOLDER}/Tests/*.cpp"

    # add necessary PathManager source files below
    # "${PM_SRC}/foo.cpp"
)

# System Manager
set(SM_FOLDER "${ROOT_DIR}/SystemManager")
set(SM_INC "${SM_FOLDER}/Inc")
set(SM_SRC "${SM_FOLDER}/Src")

include_directories(${SM_INC})
file(GLOB SM_CODE
    "${SM_FOLDER}/Tests/*.cpp"

    # add necessary SystemManager source files below
    # "${SM_SRC}/foo.cpp"
)

# Telemetry Manager
set(TM_FOLDER "${ROOT_DIR}/TelemetryManager")
set(TM_INC "${TM_FOLDER}/Inc")
set(TM_SRC "${TM_FOLDER}/Src")

include_directories(${TM_INC})
file(GLOB TM_CODE
    "${TM_FOLDER}/Tests/*.cpp"

    # add necessary TelemetryManager source files below
    "${TM_SRC}/MavlinkDecoder.cpp"
    "${TM_SRC}/MavlinkEncoder.cpp"
    "${TM_SRC}/TMCircularBuffer.cpp"
    "${TM_SRC}/MavlinkTranslator.cpp"
)

# Drivers
set(DRIVERS_FOLDER "${ROOT_DIR}/Drivers")

set(MOTORCHANNEL_INC "${DRIVERS_FOLDER}/motor_channel/inc")
set(MOTORCHANNEL_SRC "${DRIVERS_FOLDER}/motor_channel/src")

set(CIRCULAR_BUFFER_INC "${DRIVERS_FOLDER}/common/circular_buffer/inc")
set(CIRCULAR_BUFFER_SRC "${DRIVERS_FOLDER}/common/circular_buffer/src")

set(DRIVERS_CONFIG_INC "${DRIVERS_FOLDER}/Drivers/common/drivers_config/inc")

set(MOTORCHANNEL_MOCK_INC "${ROOT_DIR}/Testing/Mocks/Inc")
set(MOTORCHANNEL_MOCK_SRC "${ROOT_DIR}/Testing/Mocks/Src")

include_directories(${DRIVERS_CONFIG_INC} ${CIRCULAR_BUFFER_INC} ${MOTORCHANNEL_INC} ${MOTORCHANNEL_MOCK_INC})
file(GLOB DRIVERS_CODE
    "${DRIVERS_FOLDER}/Tests/*.cpp"

    # add necessary Drivers source files below
    # "${DRIVERS_SRC}/foo.cpp"
    "${DRIVERS_FOLDER}/common/circular_buffer/src/circular_buffer.cpp"
    "${MOTORCHANNEL_MOCK_SRC}/*.cpp"
)
# Common
set(COMMON_FOLDER "${ROOT_DIR}/Common")
set(COMMON_INC "${COMMON_FOLDER}/Inc")
set(COMMON_SRC "${COMMON_FOLDER}/Src")

include_directories(${COMMON_INC})
file(GLOB COMMON_CODE
    "${COMMON_FOLDER}/Tests/*.cpp"

    # add necessary common source files below
    # "${COMMON_SRC}/foo.cpp"
)


# Testing stubs
set(STUBS_FOLDER "${ROOT_DIR}/Testing/Stubs")
set(STUBS_INC "${STUBS_FOLDER}/Inc")
set(STUBS_SRC "${STUBS_FOLDER}/Src")

include_directories(${STUBS_INC})
file(GLOB STUBS_CODE
    "${STUBS_SRC}/*.cpp"
    "${STUBS_SRC}/*.c"
)

#Models
include_directories(${ROOT_DIR}/Config_Models)
file(GLOB MODELS_CODE
)

# If warnings are so overwhelming you can't locate the actual errors, uncomment the following line
# add_compile_options(
#     "$<$<CXX_COMPILER_ID:GNU>:-w>" # for GCC
#     "$<$<CXX_COMPILER_ID:Clang>:-w>" # for Clang
#     "$<$<CXX_COMPILER_ID:MSVC>:/W0>" # for MSVC
#   )

# --- ZeroPilot File Indexing End ---

add_executable(${PROJECT_NAME}
    ${AM_CODE}
    ${PM_CODE}
    ${SM_CODE}
    ${TM_CODE}
    ${DRIVERS_CODE}
    ${COMMON_CODE}
    ${STUBS_CODE}
    ${MODELS_CODE}
)

target_link_libraries(${PROJECT_NAME}
    GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
