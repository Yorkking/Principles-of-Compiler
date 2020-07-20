cmake_minimum_required(VERSION 2.8)

file(GLOB SOURCES
    "src/*.h"
    "src/*.cpp"
)
add_executable(
    main
    ${SOURCES}
)