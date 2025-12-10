cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target linqor_desktop)

add_library(${target} SHARED linqor_desktop.cpp desktop.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_LINUXDESKTOP)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_injection qor_filesystem qor_desktop)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
