set(EXECUTABLE_NAME Tests)

file(GLOB SOURCE_FILES ./*.cpp)

include_directories(../src)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME} PRIVATE NgRandom doctest_with_main)