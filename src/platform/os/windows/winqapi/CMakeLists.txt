cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_winapi)

add_library(${target} SHARED qor_winapi.cpp kernel/kconsole.cpp kernel/kerrorhandling.cpp kernel/kdll.cpp "kernel/khandle.cpp")

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_WINAPI)
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_error qor_issue)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")