cmake_minimum_required(VERSION 3.17)
project(Lab04)

set(CMAKE_CXX_STANDARD 20)

file(GLOB HEADER_FILES
    src/*.hpp
    src/*.inl
)

file(GLOB SOURCE_FILES
    src/*.cpp
)

include_directories(src)

add_executable(Lab04 ${HEADER_FILES} ${SOURCE_FILES})