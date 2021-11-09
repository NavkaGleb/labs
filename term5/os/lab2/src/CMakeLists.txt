set(TARGET os_lab2)

file(GLOB_RECURSE HEADER_FILES *.hpp *.inl)
file(GLOB_RECURSE SOURCE_FILES *.cpp)

add_executable(${TARGET} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE ng::random nlohmann_json tabulate)

target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "-static-libstdc++")
