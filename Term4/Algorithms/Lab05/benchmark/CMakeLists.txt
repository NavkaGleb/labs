set(TARGET Lab05_Benchmark)

file(GLOB SOURCE_FILES *.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(${TARGET} PersistentTree.cpp)

target_link_libraries(${TARGET} PRIVATE benchmark NgRandom)