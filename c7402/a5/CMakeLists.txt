cmake_minimum_required(VERSION 3.10.2)
project(feistel)

file(GLOB_RECURSE src_files "src/*.c")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/")
add_executable(feistel ${src_files})