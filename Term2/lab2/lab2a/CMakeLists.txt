cmake_minimum_required(VERSION 3.15)
project(lab2a)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab2a main.cpp ngContainers/Array.h geometry/Geometry.cpp geometry/Geometry.h ngContainers/List.h ngContainers/Deque.h ngContainers/DList.h modes/InteractiveMode.cpp modes/InteractiveMode.h modes/Benchmark.cpp modes/Benchmark.h modes/Core.cpp modes/Core.h modes/Mode.cpp modes/Mode.h ngContainers/Stack.h modes/Mode.cpp modes/DemoMode.cpp modes/DemoMode.h ngContainers/Container.h ngContainers/DequeInterface.h ngContainers/HashedArrayTree.hpp)