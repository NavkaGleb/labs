cmake_minimum_required(VERSION 3.15)
project(lab1)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab1 main.cpp entities/Monster.cpp entities/Monster.h datetime/DateTime.cpp datetime/DateTime.h database/DataBase.h database/DataBase.cpp modes/InteractiveMode.cpp modes/InteractiveMode.h modes/DemoMode.cpp modes/DemoMode.h modes/Benchmark.cpp modes/Benchmark.h location/Location.cpp location/Location.h)