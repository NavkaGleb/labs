cmake_minimum_required(VERSION 3.17)
project(lab1)

set(CMAKE_CXX_STANDARD 20)

# source files
set(SOURCE_FILES
    src/main.cpp
    include/DataStructures/StaticContainer.hpp
    include/DataStructures/DynamicContainer.hpp
    include/DataStructures/SortedContainer.hpp
    include/DataStructures/Array.hpp include/DataStructures/MatrixGraph.hpp include/DataStructures/Graph.hpp include/DataStructures/ListGraph.hpp)

# include directories
include_directories(
    "${PROJECT_SOURCE_DIR}/include/DataStructures"
    ${SFML_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})