cmake_minimum_required(VERSION 3.15)
project(lab3a)

set(CMAKE_CXX_STANDARD 17)
find_package(SFML 2.5.1 COMPONENTS system window graphics)

add_executable(
    lab3a
    main.cpp
    Sorting/algo.hpp
    Modes/DemoMode.cpp Modes/DemoMode.hpp
    Gui/RectangleShape.cpp Gui/RectangleShape.hpp
    Modes/Benchmark.cpp Modes/Benchmark.hpp
    Gui/Button.cpp Gui/Button.hpp
    Gui/Menu.cpp Gui/Menu.hpp
    Modes/Mode.hpp
    Modes/Core.cpp Modes/Core.hpp
)
target_link_libraries(lab3a sfml-system sfml-window sfml-graphics)