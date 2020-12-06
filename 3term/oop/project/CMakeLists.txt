# init project
cmake_minimum_required(VERSION 3.17)
project(project LANGUAGES C CXX CUDA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CUDA_STANDARD 14)

# init SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.5.1 COMPONENTS network window graphics system REQUIRED)

find_package(CUDAToolkit REQUIRED)

#find_package(Boost 1.74.0 COMPONENTS  REQUIRED)

# multi threading
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -march=native")

# header files
set(CXX_HEADER_FILES
    src/Application/Application.hpp
    src/States/State.hpp
    src/States/MandelbrotState.hpp
    src/Fractals/MandelbrotSet.hpp
    src/StateStack/StateStack.hpp
    src/ThreadPool/ThreadPool.hpp
    src/States/MainMenuState.hpp
    src/Gui/Button.cpp
    src/States/FluidState.hpp
        src/Random/Random.hpp)

# source files
set(CXX_SOURCE_FILES
    src/main.cpp
    src/Application/Application.cpp
    src/States/State.cpp
    src/States/MandelbrotState.cpp
    src/Fractals/MandelbrotSet.cpp
    src/StateStack/StateStack.cpp
    src/ThreadPool/ThreadPool.cpp
    src/States/MainMenuState.cpp
    src/Gui/Button.hpp
    src/States/FluidState.cpp
)

set(CUDA_HEADER_FILES
    src/Simulations/FluidSimulation/Fluid.cuh
    src/Simulations/FluidSimulation/Kernel.cuh
)

SET(CUDA_SOURCE_FILES
    src/Simulations/FluidSimulation/Fluid.cu
    src/Simulations/FluidSimulation/Kernel.cu
)

set_source_files_properties(src/Simulations/FluidSimulation/FluidPlane.cu PROPERTIES LANGUAGE "CUDA")

include_directories(src ${SFML_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${CXX_HEADER_FILES} ${CXX_SOURCE_FILES} ${CUDA_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} CUDA::cudart)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
)

#if (Boost_FOUND)
#    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
#    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
#    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
#
#    include_directories(${Boost_INCLUDE_DIRS})
#    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
#elseif()
#    message(ERROR Boost_NOT_FOUND)
#endif()