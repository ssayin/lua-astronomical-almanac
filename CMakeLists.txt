cmake_minimum_required(VERSION 2.8.2)

project(lua_aa LANGUAGES C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_subdirectory(src)
