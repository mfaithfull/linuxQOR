cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(linqor_dynamiclibrary SHARED linqor_dynamiclibrary.cpp 
dynamiclibrary.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(linqor_dynamiclibrary PRIVATE QOR_LINUXDYNLIB)
target_link_libraries(linqor_dynamiclibrary qor_module qor_host qor_datastructures qor_objectcontext qor_injection qor_dynamiclibrary qor_error)