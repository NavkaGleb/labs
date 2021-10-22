cmake_minimum_required(VERSION 3.17)
project(Lab06)

set(CMAKE_CXX_STANDARD 20)

file(GLOB HEADER_FILES src/*.hpp src/*.inl)
file(GLOB SOURCE_FILES src/*.cpp)

include_directories(src)

add_subdirectory(external/Random)
add_subdirectory(external/doctest)

add_subdirectory(tests)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE NgRandom)