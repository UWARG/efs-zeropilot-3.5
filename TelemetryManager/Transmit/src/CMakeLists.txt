# CMakeLists.txt for the src directory

# List your source files
set(SRC_FILES
        Transmit.cpp
        )

# Add an executable with the source files
add_library( main_lib STATIC ${SRC_FILES})
