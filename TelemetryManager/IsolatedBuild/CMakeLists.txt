cmake_minimum_required(VERSION 3.25)
project(ZeroPilot_SW_3_5_3)

set(CMAKE_CXX_STANDARD 14)

include_directories(../Inc)
include_directories(../Src)
include_directories(../Tests)

add_executable(ZeroPilot_SW_3_5_3
        ../Inc/MavlinkEncoder.h
        ../Inc/MavlinkDecoder.h
        ../Src/MavlinkEncoder.cpp
        ../Src/MavlinkDecoder.cpp
        ../Tests/main.cpp ../Inc/IncomingData.h)
