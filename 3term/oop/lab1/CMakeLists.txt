cmake_minimum_required(VERSION 3.17)
project(lab1)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)

# source files
set(HEADER_FILES
    include/DataStructures/StaticContainer.hpp
    include/DataStructures/DynamicContainer.hpp
    include/DataStructures/SortedContainer.hpp
    include/DataStructures/Array.hpp
    include/DataStructures/MatrixGraph.hpp
    include/DataStructures/Graph.hpp
    include/DataStructures/ListGraph.hpp
)

set(SOURCE_FILES
    src/main.cpp
    tests/MatrixGraphFixture.cpp tests/ListGraphFixture.cpp
)

# include directories
include_directories(
    "${PROJECT_SOURCE_DIR}/include/DataStructures"
    ${SFML_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# link libraries
target_link_libraries(${PROJECT_NAME} gtest gtest_main)