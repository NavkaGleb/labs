cmake_minimum_required(VERSION 3.15)
project(circles_and_curves)

set(CMAKE_CXX_STANDARD 17)

add_executable(circles_and_curves main.cpp functions.cpp functions.h)
target_link_libraries(circles_and_curves -lOpenGL32 -lfreeGLUT -lglu32)