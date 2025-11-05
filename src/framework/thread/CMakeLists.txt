cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_thread)

add_library(${target} SHARED qor_thread.cpp currentthread.cpp thread.cpp threadcontext.cpp threadpool.cpp)

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(qor_thread PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(qor_thread "${CMAKE_THREAD_LIBS_INIT}")
endif()

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_THREAD)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_sync qor_injection)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")