# init project
cmake_minimum_required(VERSION 3.17)
project(project)

set(CMAKE_CXX_STANDARD 20)

# init SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.5.1 COMPONENTS network window graphics system REQUIRED)

#find_package(Boost 1.74.0 COMPONENTS  REQUIRED)

# multi threading
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -march=native")

# header files
set(HEADER_FILES
    src/Application/Application.hpp
    src/States/State.hpp
    src/States/MandelbrotState.hpp
    src/Fractals/MandelbrotSet.hpp
    src/StateStack/StateStack.hpp
    src/ThreadPool/ThreadPool.hpp
    src/States/MainMenuState.hpp
    src/Gui/Button.cpp
)

# source files
set(SOURCE_FILES
    src/main.cpp
    src/Application/Application.cpp
    src/States/State.cpp
    src/States/MandelbrotState.cpp
    src/Fractals/MandelbrotSet.cpp
    src/StateStack/StateStack.cpp
    src/ThreadPool/ThreadPool.cpp
    src/States/MainMenuState.cpp
    src/Gui/Button.hpp
        src/States/FluidState.cpp src/States/FluidState.hpp src/Simulations/FluidSimulation/Fluid.cpp src/Simulations/FluidSimulation/Fluid.hpp)

include_directories(
    src
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
elseif()
    message(ERROR SFML_NOT_FOUND)
endif()

if (Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(${Boost_PROGRAM_OPTIONS_LIBRARY})
elseif()
    message(ERROR Boost_NOT_FOUND)
endif()