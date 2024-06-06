file(GLOB controlPins *.cpp)
target_sources(main PRIVATE ${controlPins})

target_link_libraries(main)