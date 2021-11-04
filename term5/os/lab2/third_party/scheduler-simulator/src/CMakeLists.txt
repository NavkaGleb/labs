set(TARGET scheduler_simulator)

set(PRIVATE_INCLUDE_DIR ../include/scheduler_simulator)
set(PUBLIC_INCLUDE_DIR ../include/)


file(GLOB_RECURSE HEADER_FILES ${PRIVATE_INCLUDE_DIR}/*.hpp)
file(GLOB_RECURSE SOURCE_FILES *.cpp)

add_library(${TARGET} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${TARGET} PRIVATE ${PRIVATE_INCLUDE_DIR})
target_include_directories(${TARGET} PUBLIC ${PUBLIC_INCLUDE_DIR})

target_link_libraries(${TARGET} PUBLIC ng::random tabulate)
