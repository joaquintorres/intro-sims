cmake_minimum_required(VERSION 3.10)
project(ising)

add_executable(ising main.cpp)

target_include_directories(ising PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(ising PRIVATE -Wall -Wpedantic -O3 -fomit-frame-pointer -ffast-math -march=native)
