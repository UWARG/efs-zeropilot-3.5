project(tst)
add_subdirectory(lib/googletest-main)
set(CMAKE_CXX_STANDARD 17)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


add_executable(Google_Tests_run test1.cpp)


target_link_libraries(Google_Tests_run main_lib)


target_link_libraries(Google_Tests_run gtest gtest_main)



