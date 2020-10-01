cmake_minimum_required(VERSION 3.17)
project(lab2)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    include
)

set(HEADER_FILES
    include/Solution.hpp
    include/Random.hpp
    include/Timer.hpp
)

set(SOURCE_FILES
    src/main.cpp
    src/Solution.cpp
    src/Timer.cpp
)

add_executable(lab2 ${SOURCE_FILES})