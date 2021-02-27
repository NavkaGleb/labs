cmake_minimum_required(VERSION 3.17)
project(RefactoredProject)

set(CMAKE_CXX_STANDARD 20)

file(GLOB HEADER_FILES
    src/Modes/*.hpp
)

file(GLOB SOURCE_FILES
    src/Modes/*.cpp
    src/*.cpp
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})