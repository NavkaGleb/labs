cmake_minimum_required(VERSION 3.16)
project(lab1)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(third_party/Random)

add_subdirectory(src)
add_subdirectory(test)
