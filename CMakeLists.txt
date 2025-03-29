cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(src)
add_subdirectory(test)
