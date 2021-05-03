set(TARGET Lab09_Tests)

file(GLOB SOURCE_FILES ./*.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE NgRandom doctest_with_main)