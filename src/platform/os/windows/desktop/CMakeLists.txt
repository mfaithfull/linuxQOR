cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_desktop)

add_library(${target} SHARED winqor_desktop.cpp desktop.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINDOWSDESKTOP)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_injection qor_filesystem qor_desktop
winqor_api_kernel winqor_gui)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
