cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_objectcontext)

add_library(${target} SHARED qor_objectcontext.cpp anyobject.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_OBJECTCONTEXT)
target_link_libraries(${target} qor_module qor_host)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")