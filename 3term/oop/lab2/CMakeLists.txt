cmake_minimum_required(VERSION 3.17)
project(lab2)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    include
)

# source files
set(SOURCE_FILES
    src/main.cpp
)

add_executable(lab2 ${SOURCE_FILES})