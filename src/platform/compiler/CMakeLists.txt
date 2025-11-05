cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_compiler)

add_library(${target} compiler.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")