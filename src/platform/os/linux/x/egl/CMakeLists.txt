cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)


set(target linqor_xegl)

add_library(${target} SHARED linqor_xegl.cpp eglsession.cpp eglwindow.cpp egldisplay.cpp)

target_include_directories(${target} PRIVATE)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_LINWLEGL)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_error qor_issue qor_thread 
qor_interception qor_log linq_thread linqor_x linqor_egl qor_egl
X11)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")

