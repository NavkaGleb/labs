set(TARGET os_lab1_tests)

file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/src/*.hpp ${PROJECT_SOURCE_DIR}/src/*.inl ./*.hpp)
file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp ./*.cpp)

# Remove main.cpp to exclude multiple definition of 'main' function
list(REMOVE_ITEM SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/main.cpp)

add_executable(${TARGET} ${SOURCE_FILES} ${HEADER_FILES} sandbox.cpp)

target_include_directories(${TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${TARGET} PRIVATE Ng::Random test)
