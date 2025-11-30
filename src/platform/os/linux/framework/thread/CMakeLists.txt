cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(linq_thread SHARED linq_thread.cpp currentthread.cpp)
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
target_compile_definitions(linq_thread PRIVATE -DQOR_LINUXTHREAD)
target_link_libraries(linq_thread qor_module qor_host qor_datastructures qor_objectcontext qor_injection)