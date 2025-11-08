cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_console)

add_library(${target} SHARED winq_console.cpp consolehelper.cpp console.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_WINCONSOLE)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_injection qor_error qor_issue winqor_common 
winqor_sharedbootstrap qor_filesystem)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")