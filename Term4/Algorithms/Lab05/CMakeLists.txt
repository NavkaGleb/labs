cmake_minimum_required(VERSION 3.19)
project(Lab05)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(extern/Random)
add_subdirectory(extern/doctest)

add_subdirectory(src)
add_subdirectory(tests)