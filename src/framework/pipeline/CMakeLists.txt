cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_pipeline)

add_library(${target} SHARED qor_pipeline.cpp buffer.cpp element.cpp source.cpp iosource.cpp sink.cpp
filter.cpp pipeline.cpp plug.cpp iosink.cpp  copyfilter.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_PIPELINE)
target_link_libraries(${target} qor_module qor_thread qor_datastructures qor_objectcontext qor_signals qor_sync qor_interception)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")