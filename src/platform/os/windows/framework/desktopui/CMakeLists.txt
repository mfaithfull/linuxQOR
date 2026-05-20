cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_desktopui)

add_library(${target} SHARED winqor_desktopui.cpp desktopui.cpp
)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINDOWSDESKTOPUI)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_injection 
qor_desktopui qor_error winqor_gui)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")