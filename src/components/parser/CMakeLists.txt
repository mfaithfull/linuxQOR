cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(target qor_parser)

add_library(${target} SHARED qor_parser.cpp state.cpp parser.cpp context.cpp rfc5234.cpp oneormore.cpp anyoneofset.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_PARSER)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection 
qor_workflow qor_pipeline qor_property)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
