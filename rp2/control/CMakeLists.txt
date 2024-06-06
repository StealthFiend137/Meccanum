file(GLOB control *.cpp)
target_sources(main PRIVATE ${control})

target_link_libraries(main)