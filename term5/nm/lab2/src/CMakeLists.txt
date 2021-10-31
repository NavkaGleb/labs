set(TARGET nm_lab2)

file(GLOB_RECURSE HEADER_FILES *.hpp *.inl)
file(GLOB_RECURSE SOURCE_FILES *.cpp)

add_executable(${TARGET} ${HEADER_FILES} ${SOURCE_FILES})

message(STATUS ${CMAKE_BUILD_TYPE})

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_definitions(NM_LAB2_DEBUG)
else()
    add_compile_definitions(NM_LAB2_RELEASE)
endif()