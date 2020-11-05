# init project
cmake_minimum_required(VERSION 3.17)
project(project)

set(CMAKE_CXX_STANDARD 20)

# init SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})
set(CMAKE_LIBRARY_PATH "lib/SFML")
set(SFML_INCLUDE_DIR "lib/SFML/include")
find_package(SFML 2.5.1 COMPONENTS network window graphics system REQUIRED)

# header files
set(HEADER_FILES

)

# source files
set(SOURCE_FILES
    src/main.cpp
)

include_directories(src)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
elseif ()
    message(ERROR SFML_NOT_FOUNT)
endif()
