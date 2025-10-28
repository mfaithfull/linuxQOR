cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_threadmemory SHARED qor_threadmemory.cpp
fastheap/stackpage.cpp threadheap/threadheap.cpp fastheap/fastbucket.cpp fastheap/fastheap.cpp
fastsource.cpp smallobjectheap/smallobjectpage.cpp smallobjectheap/smallobjectbucket.cpp
smallobjectheap/smallobjectheap.cpp smallobjectsource.cpp)
target_compile_definitions(qor_threadmemory PRIVATE -DQOR_THREADMEMORY)
target_link_libraries(qor_threadmemory qor_module qor_host qor_thread qor_sync)
