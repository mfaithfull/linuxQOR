cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_canvas)

add_library(${target} SHARED qor_canvas.cpp canvas.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_CANVAS)
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_role qor_application qor_error 
qor_renderer qor_opengles qor_egl)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
