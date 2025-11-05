cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_module)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-threadsafe-statics -fvisibility=hidden")
add_library(${target} library.cpp module.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")