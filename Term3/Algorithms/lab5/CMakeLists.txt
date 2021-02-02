cmake_minimum_required(VERSION 3.17)
project(lab5)

set(CMAKE_CXX_STANDARD 17)


# header files
set(HEADER_FILES
    src/Matrix/Matrix.hpp
    src/Algorithms/Strassen.hpp
    src/Algorithms/Random.hpp
    src/Benchmark/Timer.hpp
    src/Benchmark/Benchmark.hpp
    src/Console/Color.hpp
)

# source files
set(SOURCE_FILES
    src/main.cpp
    src/Console/Color.cpp
)

set(FORT_ENABLE_TESTING OFF CACHE INTERNAL "")
add_subdirectory(lib/libfort)

include_directories(
    src
    lib/libfort/lib
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE fort)