set(TARGET lab1)

file(GLOB_RECURSE HEADER_FILES ./*.hpp)
file(GLOB_RECURSE SOURCE_FILES ./*.cpp)

add_executable(${TARGET} ${SOURCE_FILES})
