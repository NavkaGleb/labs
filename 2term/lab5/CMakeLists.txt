cmake_minimum_required(VERSION 3.16)
project(lab5)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    lab5 main.cpp
    Graphs/MGraph.cpp Graphs/MGraph.hpp
    Graphs/Graph.hpp
    Random/rand.cpp Random/rand.hpp
    Graphs/LGraph.cpp Graphs/LGraph.hpp
    Trees/SpanningTree.cpp Trees/SpanningTree.hpp
    Modes/Core.cpp Modes/Core.hpp
    Modes/Mode.hpp
    Modes/Benchmark.cpp Modes/Benchmark.hpp
    Modes/ConsoleInterface.cpp Modes/ConsoleInterface.hpp TextTable/TextTable.cpp TextTable/TextTable.hpp)