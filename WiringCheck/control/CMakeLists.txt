file(GLOB control *.cpp)
target_sources(WiringCheck PRIVATE ${control})

target_link_libraries(WiringCheck)