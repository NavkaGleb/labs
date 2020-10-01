cmake_minimum_required(VERSION 3.17)
project(lab3)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    include
)

add_executable(lab3 src/main.cpp include/Heap.hpp src/Heap.cpp include/Random.hpp)