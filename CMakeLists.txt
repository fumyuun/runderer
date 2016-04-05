cmake_minimum_required(VERSION 3.0)
project(runderer)

file(GLOB sources "src/*.c")
add_library(runderer ${sources})
target_include_directories(runderer PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(runderer PUBLIC sdl2 m)
target_compile_options(runderer PRIVATE "-Weverything")

add_executable(main main.c)
target_link_libraries(main PRIVATE runderer)
