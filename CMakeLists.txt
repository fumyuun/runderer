cmake_minimum_required(VERSION 3.0)
project(runderer)

file(GLOB sources "src/*.c" "src/framebuffer/framebuffer_sdl.c")
add_library(runderer ${sources})
target_include_directories(runderer PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(runderer PUBLIC SDL2 m)
target_compile_options(runderer PRIVATE "-Weverything")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFB_SDL")

add_executable(main main.c)
target_link_libraries(main PRIVATE runderer)
