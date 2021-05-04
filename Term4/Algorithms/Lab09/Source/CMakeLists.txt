set(TARGET Lab09)

file(GLOB HEADER_FILES ./*.hpp ./*.inl)
file(GLOB SOURCE_FILES ./*.cpp)

add_executable(${TARGET} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE NgRandom fort)