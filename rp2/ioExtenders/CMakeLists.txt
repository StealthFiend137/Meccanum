file(GLOB ioExtenders *.cpp)
target_sources(main PRIVATE ${ioExtenders})

target_link_libraries(main)