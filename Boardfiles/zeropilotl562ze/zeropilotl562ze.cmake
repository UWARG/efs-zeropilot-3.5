set(FOLDER_NAME zeropilotl562ze)      
set(STARTUP_ASM zeropilotl562ze/Core/Startup/startup_stm32l562qeixq.s)      
set(LINKER_SCRIPT zeropilotl562ze/STM32L562EDK_FLASH.ld)    
set(PROJ_NAME zeropilotl562ze)    
set(FAMILY_NAME STM32L562ZETxQ)    
set(PORTABLE_NAME CM33_NTZ/non_secure)      

add_definitions(    
  -DUSE_HAL_LIB    
  -DSTM32L562ZETxQ    
  -DUSE_FULL_LL_DRIVER    
)   

# This file MUST be included as a toolchain file in cmake for the build to work    
# Use the -DCMAKE_TOOLCHAIN_FILE flag in cmake    
# Sometimes we need to compile for x86 for things like unit tests, in which case this file    
# shouldn't be included in the toolchain    
set(CMAKE_SYSTEM_NAME Generic)    
set(CMAKE_SYSTEM_PROCESSOR arm)    
set(CMAKE_SYSTEM_VERSION 1)    

# specify the cross compiler    
set(CMAKE_C_COMPILER arm-none-eabi-gcc)    
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)    
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)    
set(CMAKE_OBJCOPY arm-none-eabi-objcopy CACHE INTERNAL "")    
set(CMAKE_SIZE arm-none-eabi-size CACHE INTERNAL "")

# ARM compiler options    
set(MCU_CPU cortex-m33)    
set(MCU_FLOAT_ABI hard)   
set(MCU_FLAGS "-mcpu=${MCU_CPU} -mthumb -mfloat-abi=${MCU_FLOAT_ABI}")    

set(COMMON_FLAGS "${MCU_FLAGS} -g -Wall -Wextra -Wno-unused-parameter -ffunction-sections -fdata-sections")    
    
set(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=gnu11" CACHE INTERNAL "")    
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -Wno-implicit-fallthrough -Wno-register -std=c++17 -fno-rtti -fno-exceptions -fno-unwind-tables" CACHE INTERNAL "")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS} -x assembler-with-cpp" CACHE INTERNAL "")    
set(CMAKE_EXE_LINKER_FLAGS "${MCU_FLAGS} -specs=nosys.specs -specs=nano.specs -Wl,-gc-sections" CACHE INTERNAL "")    
    
# insert DEBUG flag if we're compiling under debug    
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -DDEBUG")    
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -DDEBUG")    
    
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)    
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)    
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)    
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)    
