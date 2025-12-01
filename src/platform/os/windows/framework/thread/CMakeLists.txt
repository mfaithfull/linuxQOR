cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_thread)

add_library(${target} SHARED winqor_thread.cpp currentthread.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_WINDOWSTHREAD)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_injection qor_thread qor_sync qor_error qor_filesystem)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")