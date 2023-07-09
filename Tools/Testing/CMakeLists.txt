cmake_minimum_required(VERSION 3.18)
project(unit_testing)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
set(AM_INC "{$AM_FOLDER}/Inc")
set(AM_SRC "{$AM_FOLDER}/Src")

include_directories(${AM_INC})
file(GLOB AM_CODE
    "${AM_FOLDER}/Tests/*.cpp"

    # add necessary AttitudeManager source files below
    # "${AM_SRC}/foo.cpp"
)

# Path Manager
set(PM_FOLDER "${ROOT_DIR}/PathManager")
set(PM_INC "{$PM_FOLDER}/Inc")
set(PM_SRC "{$PM_FOLDER}/Src")

include_directories(${PM_INC})
file(GLOB PM_CODE
    "${PM_FOLDER}/Tests/*.cpp"

    # add necessary PathManager source files below
    # "${PM_SRC}/foo.cpp"
)

# System Manager
set(SM_FOLDER "${ROOT_DIR}/SystemManager")
set(SM_INC "{$SM_FOLDER}/Inc")
set(SM_SRC "{$SM_FOLDER}/Src")

include_directories(${SM_INC})
file(GLOB SM_CODE
    "${SM_FOLDER}/Tests/*.cpp"

    # add necessary SystemManager source files below
    # "${SM_SRC}/foo.cpp"
)

# Telemetry Manager
set(TM_FOLDER "${ROOT_DIR}/TelemetryManager")
set(TM_INC "{$TM_FOLDER}/Inc")
set(TM_SRC "{$TM_FOLDER}/Src")

include_directories(${TM_INC})
file(GLOB TM_CODE
    "${TM_FOLDER}/Tests/*.cpp"

    # add necessary TelemetryManager source files below
    # "${TM_SRC}/foo.cpp"
)

# Drivers
set(DRIVERS_FOLDER "${ROOT_DIR}/Drivers")
set(DRIVERS_INC "${DRIVERS_FOLDER}/Common/Inc")
set(DRIVERS_SRC "{$DRIVERS_FOLDER}/Src")

include_directories(${DRIVERS_INC})
file(GLOB DRIVERS_CODE
    "${DRIVERS_FOLDER}/Tests/*.cpp"

    # add necessary Drivers source files below
    # "${DRIVERS_SRC}/foo.cpp"
    "${DRIVERS_FOLDER}/Common/Src/circular_buffer.cpp"
)

# --- ZeroPilot File Indexing End ---

add_executable(${PROJECT_NAME}
    ${AM_CODE}
    ${PM_CODE}
    ${SM_CODE}
    ${TM_CODE}
    ${DRIVERS_CODE}
)

target_link_libraries(${PROJECT_NAME}
    GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
