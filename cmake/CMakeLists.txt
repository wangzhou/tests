cmake_minimum_required(VERSION 3.16)
project(hello_demo)

add_executable(hello hello.c hello.h)

add_subdirectory(lib)

target_link_libraries(hello add numa)
set(CMAKE_EXE_LINKER_FLAGS -L/opt)
