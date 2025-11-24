cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_filesink)

add_library(${target} SHARED qor_filesink.cpp filesink.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(qor_filesink PRIVATE QOR_FILESINK)
target_link_libraries(qor_filesink qor_module qor_datastructures 
qor_objectcontext qor_signals qor_pipeline qor_platform
qor_filesystem qor_error)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")