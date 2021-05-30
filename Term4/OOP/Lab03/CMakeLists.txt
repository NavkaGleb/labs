cmake_minimum_required(VERSION 3.19)
project(Lab03)

set(CMAKE_CXX_STANDARD 20)

#set(ZIBEN_ENABLE_PROFILING TRUE)
add_subdirectory(External/Ziben)
add_subdirectory(External/doctest)

add_subdirectory(Source)
add_subdirectory(Tests)