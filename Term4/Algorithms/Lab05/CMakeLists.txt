cmake_minimum_required(VERSION 3.19)
project(Lab05)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(extern/Random)
add_subdirectory(extern/doctest)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
add_subdirectory(extern/benchmark)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(benchmark)