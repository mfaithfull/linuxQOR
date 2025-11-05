cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_stdoutsink)

add_library(${target} SHARED qor_stdoutsink.cpp stdoutsink.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_STDOUTSINK)
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_signals qor_pipeline)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")