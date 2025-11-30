cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_workflow)

add_library(${target} SHARED qor_workflow.cpp workflow.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_WORKFLOW)
target_link_libraries(${target} qor_module qor_thread qor_datastructures qor_objectcontext qor_error qor_log)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")