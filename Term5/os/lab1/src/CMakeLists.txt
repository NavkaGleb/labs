set(TARGET os_lab1)

file(GLOB_RECURSE HEADER_FILES ./*.hpp ./*.inl)
file(GLOB_RECURSE SOURCE_FILES ./*.cpp)

add_executable(${TARGET} ${HEADER_FILES} ${SOURCE_FILES} computation_function.hpp nonblocking_read_poll.hpp nonblocking_read_poll.cpp keyboard.cpp keyboard.hpp)

target_link_libraries(${TARGET} PRIVATE ncurses)
