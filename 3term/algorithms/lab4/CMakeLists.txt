cmake_minimum_required(VERSION 3.17)
project(lab4)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    src
)

# header files
set(HEADER_FILES
    src/Solution.hpp
    src/Random.hpp
    src/ConsoleColor.hpp
)

# source files
set(SOURCE_FILES
    src/main.cpp
    src/ConsoleColor.cpp
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

