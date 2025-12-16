cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_termscreen)

add_library(${target} SHARED winq_termscreen.cpp termscreen.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINTERMSCREEN)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_injection qor_error qor_issue winqor_common 
winqor_sharedbootstrap qor_tui)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")