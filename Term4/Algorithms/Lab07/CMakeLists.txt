cmake_minimum_required(VERSION 3.17)
project(Lab07)

set(CMAKE_CXX_STANDARD 20)

file(GLOB HEADER_FILES Source/*.hpp Source/*.inl)
file(GLOB SOURCE_FILES Source/*.cpp)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})