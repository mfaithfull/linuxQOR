cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_interception)

add_library(${target} SHARED qor_interception.cpp callcontext.cpp functioncontext.cpp
 flystrapbase.cpp flystrap.cpp callinterceptor.cpp functioncontextlock.cpp
 parameters/return.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_INTERCEPTION)
target_link_libraries(${target} qor_module qor_host qor_objectcontext qor_thread)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")