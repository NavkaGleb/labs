set(TARGET SortAlgorithms_Benchmark)

file(GLOB SOURCE_FILES ./*.cpp)

add_subdirectory(QuickSortBenchmark)
add_subdirectory(ParallelQuickSortBenchmark)
add_subdirectory(MergeSortBenchmark)
add_subdirectory(ParallelMergeSortBenchmark)

add_executable(${TARGET} ${SOURCE_FILES})

target_include_directories(${TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/Source)

target_link_libraries(${TARGET} PRIVATE benchmark ZibenUtility)