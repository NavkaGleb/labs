cmake_minimum_required(VERSION 3.19)
project(Lab03)

set(CMAKE_CXX_STANDARD 20)

# Own GameEngine
#set(ZIBEN_ENABLE_PROFILING TRUE)
add_subdirectory(External/Ziben)

# Testing Libarary
add_subdirectory(External/doctest)

# Benchmark Library
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
add_subdirectory(External/benchmark)

add_subdirectory(Source)
add_subdirectory(Tests)
add_subdirectory(Benchmark)