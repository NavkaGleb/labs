cmake_minimum_required(VERSION 3.15)
project(lab4)

set(CMAKE_CXX_STANDARD 17)

add_executable(
    lab4 main.cpp
    Trees/GeneralTree.hpp
    Rand/rand.hpp
    Trees/BinarySearchTree.hpp
    Trees/ExpressionTree.cpp Trees/ExpressionTree.hpp
    Modes/Core.cpp Modes/Core.hpp Modes/Mode.hpp Trees/Tree.hpp Modes/Mode.cpp Rand/rand.cpp)