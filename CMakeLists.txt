#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.22)

# specify cross-compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(truck_hw C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

#Uncomment for hardware floating point
add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16 --specs=nano.specs --specs=nosys.specs)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0 -fno-threadsafe-statics)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

# Enable assembler files preprocessing
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

message(STATUS "Maximum optimization for speed")
add_compile_options(-Og)

#if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
#    message(STATUS "Maximum optimization for speed")
#    add_compile_options(-Ofast)
#elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
#    message(STATUS "Maximum optimization for speed, debug info included")
#    add_compile_options(-Ofast -g)
#elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
#    message(STATUS "Maximum optimization for size")
#    add_compile_options(-Os)
#else ()
#    message(STATUS "Minimal optimization, debug info included")
#    add_compile_options(-Og -g)
#endif ()

include_directories(Core/Inc Drivers/STM32G4xx_HAL_Driver/Inc Drivers/STM32G4xx_HAL_Driver/Inc/Legacy Drivers/CMSIS/Device/ST/STM32G4xx/Include Drivers/CMSIS/Include
        board/ periphery/uart periphery/gpio periphery/spi periphery/pwm6 periphery/fdcan periphery/pwm_servo/include
        chipset/tle5012 chipset/icm20984/include chipset/Servo/include
        portSimpleFOC/Sensor portSimpleFOC/Driver SimpleFOC/src/common SimpleFOC/src/drivers SimpleFOC/src
        middleware/FreeRTOS/include middleware/FreeRTOS/ARM_CM4F utils/
        application/MotorControl/include application/SensorPolling/include application/Protocol/include application/ServoController/include)

add_definitions(-DDEBUG -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32G431xx)

file(GLOB SIMPLEFOC_SOURCES "SimpleFOC/src/common/foc_utils.*"
        "SimpleFOC/src/common/pid.*" "SimpleFOC/src/common/lowpass_filter.*" "SimpleFOC/src/common/defaults.h"
        "SimpleFOC/src/common/base_classes/Sensor.*" "SimpleFOC/src/common/base_classes/BLDCDriver.*"
        "SimpleFOC/src/common/base_classes/FOCMotor.*" "SimpleFOC/src/BLDCMotor.*"
        "SimpleFOC/src/common/base_classes/CurrentSense.*")

file(GLOB_RECURSE SOURCES "Core/*.*" "Drivers/*.*" "board/*" "periphery/gpio/*" "periphery/uart/*" "periphery/pwm6/*"
        "periphery/spi/*" "periphery/fdcan/*" "periphery/pwm_servo/*" "chipset/tle5012/*" "chipset/icm20984/sources/*" "chipset/Servo/Servo.cpp"
        "portSimpleFOC/Sensor/*" "portSimpleFOC/Driver/*" "portSimpleFOC/time_utils.cpp"
        "application/MotorControl/sources/*" "application/SensorPolling/sources/*" "application/Protocol/*"
        "utils/*" "middleware/FreeRTOS/*" "application/ServoController/*")

set(SOURCES ${SOURCES} ${SIMPLEFOC_SOURCES})

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32G431RBTX_FLASH.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
