cmake_minimum_required(VERSION 3.17)
project(lab1 C)

set(CMAKE_C_STANDARD 11)

include_directories(
    include
    include/structs
    lib/text_table
)

# set HEADER FILES
set(HEADER_FILES
    include/functions.h
)

# set SOURCE FILES
set(SOURCE_FILES
    src/main.c
    src/functions.c include/structs/country.h include/structs/index_country.h include/structs/city.h include/structs/index_city.h include/structs/global_data.h src/user_interface.c include/user_interface.h lib/text_table/fort.c)

add_executable(lab1 ${SOURCE_FILES})