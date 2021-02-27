cmake_minimum_required(VERSION 3.17)
project(RefactoredProject)

set(CMAKE_CXX_STANDARD 20)

file(GLOB HEADER_FILES
    src/Application/*.hpp
    src/DataBase/*.hpp
    src/Entities/*.hpp
    src/Modes/*.hpp
    src/Utility/*.hpp
)

file(GLOB SOURCE_FILES
    src/Application/*.cpp
    src/DataBase/*.cpp
    src/Entities/*.cpp
    src/Modes/*.cpp
    src/*.cpp
)

include_directories(src)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})