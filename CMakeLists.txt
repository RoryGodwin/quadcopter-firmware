cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(quadcopter_firmware C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(quadcopter_firmware
    src/main.cpp
    src/Motor_Control.cpp
)

target_include_directories(quadcopter_firmware PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(quadcopter_firmware
    pico_stdlib
    hardware_pwm
)

pico_add_extra_outputs(quadcopter_firmware)