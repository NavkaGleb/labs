set(TARGET SortAlgorithms_ParallelQuickSortBenchmark)

file(GLOB SOURCE_FILES ./*.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_include_directories(${TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/Source)

target_link_libraries(${TARGET} PRIVATE benchmark ZibenUtility)