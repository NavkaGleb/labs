cmake_minimum_required(VERSION 3.15)
project(lab3b)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    lab3b main.cpp
    Modes/InteractiveMode.cpp
    Modes/InteractiveMode.hpp
    Modes/Benchmark.cpp
    Modes/Benchmark.hpp
    Modes/Mode.hpp
    Modes/Core.cpp
    Modes/Core.hpp
    algo/algo.hpp
    Entities/Monster.hpp
    Entities/Monster.cpp
)