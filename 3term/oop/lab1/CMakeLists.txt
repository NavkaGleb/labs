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
        include/DataStructures/Graphs/MatrixGraph.hpp
        include/DataStructures/Graphs/Graph.hpp
        include/DataStructures/Graphs/ListGraph.hpp
)

set(SOURCE_FILES
    src/main.cpp
    tests/MatrixGraphFixture.cpp tests/ListGraphFixture.cpp
        include/FileSystem/File.hpp include/DateTime/Date.hpp include/DateTime/Time.hpp include/FileSystem/Directory.hpp include/FileSystem/FileSystemObject.hpp include/DataStructures/Trees/GeneralTree.hpp include/Random/Random.hpp include/FileSystem/FileSystem.hpp src/FileSystem/FileSystem.cpp src/FileSystem/File.cpp src/FileSystem/FileSystemObject.cpp src/FileSystem/Directory.cpp)

# include directories
include_directories(
    include
    /include/DataStructures
    ${SFML_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# link libraries
target_link_libraries(${PROJECT_NAME} gtest gtest_main)