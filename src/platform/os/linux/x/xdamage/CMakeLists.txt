cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target linqor_xdamage)

add_library(${target} SHARED linqor_damage.cpp damage.cpp)

target_include_directories(${target} PRIVATE)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_LINXDAMAGE)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_error qor_issue qor_thread 
qor_interception linq_thread linqor_x X11)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")

