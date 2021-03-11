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

add_subdirectory(lib/Random)
add_subdirectory(lib/libfort)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE NgRandom libfort::fort)