cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_filesystem)

add_library(${target} SHARED winqor_filesystem.cpp filesystem.cpp filestat.cpp file.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_WINDOWSFILESYSTEM)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_injection qor_filesystem)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
