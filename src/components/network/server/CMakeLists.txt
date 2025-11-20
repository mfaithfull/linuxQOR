cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_netserver)

add_library(${target} SHARED qor_netserver.cpp netserver.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_NETSERVER)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection qor_log qor_error)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
