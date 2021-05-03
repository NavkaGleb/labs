cmake_minimum_required(VERSION 3.19)
project(Lab09)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(External/doctest)
add_subdirectory(External/Random)

add_subdirectory(Source)
add_subdirectory(Tests)