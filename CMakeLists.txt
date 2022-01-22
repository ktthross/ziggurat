cmake_minimum_required(VERSION 3.16)

project(Ziggurat)

add_library(zig_random zig_random.cpp)
add_library(ziggurat ziggurat.cpp)
target_link_libraries(ziggurat zig_random)

add_executable(main_zig zig_handler.cpp)
target_link_libraries(main_zig ziggurat)
