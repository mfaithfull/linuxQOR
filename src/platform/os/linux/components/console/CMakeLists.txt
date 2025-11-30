cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(linq_console SHARED linq_console.cpp console.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(linq_console PRIVATE -DQOR_LINCONSOLE)
target_link_libraries(linq_console qor_module qor_host qor_datastructures qor_objectcontext qor_injection qor_error qor_issue)