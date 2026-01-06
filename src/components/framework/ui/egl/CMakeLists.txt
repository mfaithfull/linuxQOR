cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_egl)

add_library(${target} SHARED qor_egl.cpp egl.cpp display.cpp context.cpp window.cpp session.cpp surface.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_EGL)
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_role qor_application qor_error)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
