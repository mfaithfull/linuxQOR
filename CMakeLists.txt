cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_COMPILE_OPTIONS_RTTI)
set(CMAKE_CXX_RTTI ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(examples)
