file(GLOB motors *.cpp)
target_sources(main PRIVATE ${motors})

target_link_libraries(main hardware_pwm)