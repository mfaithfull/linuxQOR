cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winq_process)

add_library(${target} SHARED winq_process.cpp currentprocess.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_WINDOWSPROCESS)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_injection qor_thread qor_issue qor_error qor_filesystem)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")