cmake_minimum_required(VERSION 3.12.0)
project(ZeroPilot C CXX ASM)

set(ELF_NAME ${PROJECT_NAME}.elf)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

## TODO: Make this configurable via build script
Set(MODEL_NAME, testmodel1)

include_directories(
    ## Common Includes ##
    ${ROOT_DIR}/Common/Inc

    ## System Manager Includes ##
    ${ROOT_DIR}/SystemManager/Inc

    ## Attitude Manager Includes ##
    ${ROOT_DIR}/AttitudeManager/Inc
    ${ROOT_DIR}/AttitudeManager/FlightModes/Inc

    ## Path Manager Includes ##

    ## Telemetry Manager Includes ##

    ## Driver Includes ##
    ${ROOT_DIR}/Drivers/rc_receiver/Inc
    ${ROOT_DIR}/Drivers/Common/Inc

    ${ROOT_DIR}/Drivers/ccontrol/Inc
    ${ROOT_DIR}/Drivers/sensor_fusion/Inc

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
    
    ## Models Includes ##
    ${ROOT_DIR}/Models
    ${ROOT_DIR}/Models/${MODEL_NAME}
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

## Models Sources ##
set(MODELS ${ROOT_DIR}/Models)
set(MODELS_C_SOURCES "${MODELS}/*.c"
                    "${MODELS}/${MODEL_NAME}/*.c")
set(MODELS_CXX_SOURCES "${MODELS}/*.cpp"
                      "${MODELS}/${MODEL_NAME}/*.cpp")

## Zeropilot Sources ##
set(ATTITUDE_MANAGER ${ROOT_DIR}/AttitudeManager)
set(ATTITUDE_MANAGER_C_SOURCES "${ATTITUDE_MANAGER}/Src/*.c"
                              "${ATTITUDE_MANAGER}/FlightModes/Src/*.c")
set(ATTITUDE_MANAGER_CXX_SOURCES "${ATTITUDE_MANAGER}/Src/*.cpp"
                                "${ATTITUDE_MANAGER}/FlightModes/Src/*.cpp")

set(SYSTEM_MANAGER ${ROOT_DIR}/SystemManager)
set(SYSTEM_MANAGER_C_SOURCES "${SYSTEM_MANAGER}/Src/*.c")
set(SYSTEM_MANAGER_CXX_SOURCES "${SYSTEM_MANAGER}/Src/*.cpp")

set(DRIVERS_DIR ${ROOT_DIR}/Drivers)
set(DRIVERS_C_SOURCES "${DRIVERS_DIR}/ccontrol/Src/*.c")
set(DRIVERS_CXX_SOURCES "${DRIVERS_DIR}/sensor_fusion/Src/*.cpp")

## Drivers Sources ##
set(DRIVERS_RcReceiver_DIR ${ROOT_DIR}/Drivers/rc_receiver/Src)
set(DRIVERS_RcReceiver_CXX_SOURCES "${DRIVERS_RcReceiver_DIR}/*.cpp")
set(DRIVERS_Common_DIR ${ROOT_DIR}/Drivers/Common/Src)
set(DRIVERS_Common_CXX_SOURCES "${DRIVERS_Common_DIR}/*.cpp")

## Actually find the sources, NOTE: if you add a new source above, add it here! ##
file(GLOB_RECURSE C_SOURCES ${HAL_DRIVERS_C_SOURCES}
                            ${FREE_RTOS_C_SOURCES}
                            ${HAL_CORE_C_SOURCES}
                            ${ATTITUDE_MANAGER_C_SOURCES}
                            ${SYSTEM_MANAGER_C_SOURCES}
                            ${DRIVERS_C_SOURCES})
message("C Sources: ${C_SOURCES}")
file(GLOB_RECURSE CXX_SOURCES ${HAL_CORE_CXX_SOURCES}
                              ${ATTITUDE_MANAGER_CXX_SOURCES}
                              ${SYSTEM_MANAGER_CXX_SOURCES}
                              ${DRIVERS_RcReceiver_CXX_SOURCES}
                              ${DRIVERS_Common_CXX_SOURCES}
                              ${DRIVERS_CXX_SOURCES})
message("C++ Sources: ${CXX_SOURCES}")

## Find the startup and linker script ##
set(STARTUP_ASM_FILE ${ROOT_DIR}/Boardfiles/${STARTUP_ASM})
set(LINKER_SCRIPT_FILE ${ROOT_DIR}/Boardfiles/${LINKER_SCRIPT})

add_executable(${ELF_NAME} ${C_SOURCES} ${CXX_SOURCES} ${STARTUP_ASM_FILE})

# Specify C standard we use
set_property(TARGET ${ELF_NAME} PROPERTY C_STANDARD 11)
set_property(TARGET ${ELF_NAME} PROPERTY CXX_STANDARD 20)

# Error on warnings, enable all warnings
#target_compile_options(${ELF_NAME} PRIVATE
#  -Werror -Wall -Wextra -Wpedantic
#)

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