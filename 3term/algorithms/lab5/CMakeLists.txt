cmake_minimum_required(VERSION 3.17)
project(lab5)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    src
)

# header files
set(HEADER_FILES
    src/Matrix.hpp
        src/Strassen.hpp)

# source files
set(SOURCE_FILES
    src/main.cpp
        src/Random.hpp)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})