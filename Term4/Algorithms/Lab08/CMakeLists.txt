cmake_minimum_required(VERSION 3.19)
project(Lab08)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external/Random)
add_subdirectory(external/doctest)

add_subdirectory(src)
add_subdirectory(tests)