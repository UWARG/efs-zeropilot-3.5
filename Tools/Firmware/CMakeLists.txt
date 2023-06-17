cmake_minimum_required(VERSION 3.2.0)
project(ZeroPilot C CXX ASM)

set(ELF_NAME ${PROJECT_NAME}.elf)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

include_directories(
    ## System Manager Includes ##

    ## Attitude Manager Includes ##

    ## Path Manager Includes ##

    ## Telemetry Manager Includes ##

    ## Driver Includes ##

    ## Boardfiles Includes ##
    ${ROOT_DIR}/Boardfiles/${FOLDER_NAME}/Drivers/${FAMILY_NAME}_HAL_Driver/Inc
    ${ROOT_DIR}/Boardfiles/${FOLDER_NAME}/Core/Inc
    ${ROOT_DIR}/Boardfiles/${FOLDER_NAME}/Drivers/CMSIS/Device/ST/${FAMILY_NAME}/Include
    ${ROOT_DIR}/Boardfiles/${FOLDER_NAME}/Drivers/CMSIS/Include
    ${ROOT_DIR}/Boardfiles/${FOLDER_NAME}/Middlewares/Third_Party/FreeRTOS/Source
    ${ROOT_DIR}/Boardfiles/${FOLDER_NAME}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
    ${ROOT_DIR}/Boardfiles/${FOLDER_NAME}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    ${ROOT_DIR}/Boardfiles/${FOLDER_NAME}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_${PORTABLE_NAME}
    ${ROOT_DIR}/Boardfiles/${FOLDER_NAME}/Middlewares/Third_Party/FreeRTOS/Source/include
)

## Boardfile Sources ##
set(HAL_DRIVERS_DIR ${ROOT_DIR}/Boardfiles/${FOLDER_NAME}/Drivers)
set(HAL_DRIVERS_C_SOURCES "${HAL_DRIVERS_DIR}/${FAMILY_NAME}_HAL_Driver/Src/*.c")

set(FREE_RTOS_DIR ${ROOT_DIR}/Boardfiles/${FOLDER_NAME}/Middlewares/Third_Party/FreeRTOS/Source)
set(FREE_RTOS_C_SOURCES "${FREE_RTOS_DIR}/*.c"
                        "${FREE_RTOS_DIR}/CMSIS_RTOS_V2/*.c"
                        "${FREE_RTOS_DIR}/portable/GCC/ARM_${PORTABLE_NAME}/*.c"
                        "${FREE_RTOS_DIR}/portable/MemMang/*.c")

set(HAL_CORE ${ROOT_DIR}/Boardfiles/${FOLDER_NAME}/Core)
set(HAL_CORE_C_SOURCES "${HAL_CORE}/Src/*.c")
set(HAL_CORE_CXX_SOURCES "${HAL_CORE}/Src/*.cpp")

## Actually find the sources, NOTE: if you add a new source above, add it here! ##
file(GLOB_RECURSE C_SOURCES ${HAL_DRIVERS_C_SOURCES}
                            ${FREE_RTOS_C_SOURCES}
                            ${HAL_CORE_C_SOURCES})                            
message("MESSAGE: ${C_SOURCES}")                         
file(GLOB_RECURSE CXX_SOURCES ${HAL_CORE_CXX_SOURCES})

## Find the startup and linker script ##
set(STARTUP_ASM_FILE ${ROOT_DIR}/Boardfiles/${STARTUP_ASM})
set(LINKER_SCRIPT_FILE ${ROOT_DIR}/Boardfiles/${LINKER_SCRIPT})

add_executable(${ELF_NAME} ${C_SOURCES} ${CXX_SOURCES} ${STARTUP_ASM_FILE})

# Add project-specific linker flags (.ld script, .map file)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${LINKER_SCRIPT_FILE} -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map,--cref")

set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

# objcopy the .elf file to .bin file for programming
add_custom_target("${PROJECT_NAME}.bin" ALL
  COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${ELF_NAME}> ${BIN_FILE}
  DEPENDS ${ELF_NAME}
)

# Print build type
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug build.")
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    message("Release build.")
ELSE()
    message("Some other build type.")
ENDIF()

# Print size information after compiling
add_custom_command(TARGET ${ELF_NAME}
  POST_BUILD
  COMMENT "Binary size information:"
  COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${ELF_NAME}>
)