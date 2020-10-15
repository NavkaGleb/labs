cmake_minimum_required(VERSION 3.17)
project(lab4)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    include/
)

# header files
set(HEADER_FILES

)

# source files
set(SOURCE_FILES
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})