cmake_minimum_required(VERSION 3.17)
project(lab6)

set(CMAKE_CXX_STANDARD 20)

set(HEADER_FILES
    src/Random.hpp
    src/Random.hpp
)

set(SOURCE_FILES
    src/main.cpp
    src/IsCyclical.cpp
)

# include directories
include_directories(src)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})