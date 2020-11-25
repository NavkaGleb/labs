cmake_minimum_required(VERSION 3.17)
project(lab7)

set(CMAKE_CXX_STANDARD 20)

set(HEADER_FILES
        src/Matrix/Matrix.hpp)

set(SOURCE_FILES
    src/main.cpp
)

include_directories(src)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})