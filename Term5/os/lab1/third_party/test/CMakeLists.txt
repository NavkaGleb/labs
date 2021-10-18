set(TARGET test)

file(GLOB_RECURSE HEADER_FILES include/test/*.hpp)

add_library(${TARGET} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${TARGET} PUBLIC include)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)
