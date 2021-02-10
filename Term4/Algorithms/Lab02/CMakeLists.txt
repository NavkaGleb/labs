cmake_minimum_required(VERSION 3.17)
project(Lab02)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

# NgRandom library
add_subdirectory(lib/Random)

file(GLOB HEADER_FILES
    src/*.hpp
    src/*.inl
)

file(GLOB SOURCE_FILES
    src/*.cpp
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} NgRandom)