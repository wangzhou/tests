project(hello_demo)

add_executable(hello hello.c hello.h)

add_subdirectory(lib)

target_link_libraries(hello add)
