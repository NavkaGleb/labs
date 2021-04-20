set(TARGET ${PROJECT_NAME})

file(GLOB HEADER_FILES ./*.hpp ./*.inl)
file(GLOB SOURCE_FILES ./*.cpp)

add_executable(${TARGET} ${HEADER_FILES} ${SOURCE_FILES})