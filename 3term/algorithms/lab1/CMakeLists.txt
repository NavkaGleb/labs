cmake_minimum_required(VERSION 3.17)
project(lab1)

set(CMAKE_CXX_STANDARD 20)

# include directories
include_directories(
    "${PROJECT_SOURCE_DIR}/include"
    ${SFML_INCLUDE_DIR}
)

add_executable(lab1 src/main.cpp include/Tape.hpp include/PolyphaseMergeSort.hpp include/Random.hpp)