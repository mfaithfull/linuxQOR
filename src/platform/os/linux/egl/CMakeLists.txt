cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)


set(target linqor_egl)

add_library(${target} SHARED linqor_egl.cpp egl.cpp display.cpp)

target_include_directories(${target} PRIVATE)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_LINEGL qor_pp_egl_backend=qor_pp_use_x)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_error qor_issue qor_thread 
qor_interception qor_egl linq_thread EGL X11)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")

add_subdirectory(examples)
