cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_platform)

add_library(${target} SHARED qor_platform.cpp platform.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_PLATFORM)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_sync qor_thread)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")

add_subdirectory(compiler)
add_subdirectory(architecture)
add_subdirectory(os)
add_subdirectory(filesystem)
add_subdirectory(network)

