cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_console)

add_library(${target} SHARED qor_console.cpp console.cpp consolerenderer.cpp consoleview.cpp 
consoleviewcontroller.cpp consoleviewmodel.cpp tokenbuffer.cpp consolecompositor.cpp consolelabelwidget.cpp
consoleui.cpp consolewidget.cpp consolecolumnwidget.cpp consolemenuwidget.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_CONSOLE)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection qor_role)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")