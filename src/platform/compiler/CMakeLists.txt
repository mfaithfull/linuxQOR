cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_compiler)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_library(${target} compiler.cpp)

target_compile_definitions(${target} PRIVATE -DQOR_COMPILER)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")