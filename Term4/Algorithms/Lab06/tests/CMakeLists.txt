file(GLOB SOURCE_FILES *.cpp)

include_directories(../src)

add_executable(Tests ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(Tests PRIVATE NgRandom doctest_with_main )