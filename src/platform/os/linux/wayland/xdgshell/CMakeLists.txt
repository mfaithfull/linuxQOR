cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)


set(target linqor_xdgshell)

add_library(${target} SHARED linqor_xdgshell.cpp xdg-shell.c xdgwmbase.cpp xdgsurface.cpp
xdgtoplevel.cpp xdgpopup.cpp xdgpositioner.cpp
listeners/xdgsurfacelistener.cpp
listeners/xdgtoplevellistener.cpp
listeners/xdgwmbaselistener.cpp
listeners/xdgpopuplistener.cpp
xdgsession.cpp
xdgwindow.cpp
xdgtoplevelwindow.cpp
xdgpopupwindow.cpp
)

target_include_directories(${target} PRIVATE)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_LINWLXDGSHELL)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_error qor_issue qor_thread 
qor_interception qor_log linq_thread linqor_waylandclient wayland-client)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")

