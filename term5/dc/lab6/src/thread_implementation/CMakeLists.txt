set(TARGET dc_lab6_thread)

file(GLOB HEADER_FILES ../*.hpp ../*.inl *.hpp *.inl)
file(GLOB SOURCE_FILES ../*.cpp *.cpp)

add_executable(${TARGET} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE ng::random tabulate)

target_include_directories(${TARGET} PRIVATE ../)
