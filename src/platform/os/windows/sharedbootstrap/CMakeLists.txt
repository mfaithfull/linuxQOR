cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqsharedbootstrap)

add_library(winqsharedbootstrap winqsharedbootstrap.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_link_libraries(${target})

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")