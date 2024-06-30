file(GLOB ioExtenders *.cpp)
target_sources(WiringCheck PRIVATE ${ioExtenders})

target_link_libraries(WiringCheck)