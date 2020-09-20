cmake_minimum_required(VERSION 3.17)
project(lab1 C)

set(CMAKE_C_STANDARD 11)

include_directories(
    include
    include/structs
)

# set HEADER FILES
set(HEADER_FILES
    include/functions.h
)

# set SOURCE FILES
set(SOURCE_FILES
    src/main.c
    src/functions.c include/structs/person.h)

add_executable(lab1 ${SOURCE_FILES})