cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(chassis_control C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(chassis_control
    main.c
)

target_link_libraries(chassis_control
    pico_i2c_slave
    hardware_i2c
    pico_stdlib
    )

pico_add_extra_outputs(chassis_control)
