cmake_minimum_required(VERSION 3.24)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp)
add_executable(main_run ${SOURCE_FILES})

include_directories(src)
add_subdirectory(src)

target_link_libraries(main_run main_lib)

add_subdirectory(tst)


