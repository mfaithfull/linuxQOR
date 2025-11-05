cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_threadmemory)

add_library(${target} SHARED qor_threadmemory.cpp
fastheap/stackpage.cpp threadheap/threadheap.cpp fastheap/fastbucket.cpp fastheap/fastheap.cpp
fastsource.cpp smallobjectheap/smallobjectpage.cpp smallobjectheap/smallobjectbucket.cpp
smallobjectheap/smallobjectheap.cpp smallobjectsource.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_THREADMEMORY)
target_link_libraries(${target} qor_module qor_host qor_thread qor_sync)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
