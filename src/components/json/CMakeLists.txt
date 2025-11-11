cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(target qor_json)

add_library(${target} SHARED qor_json.cpp nodes/string.cpp nodes/value.cpp nodes/object.cpp nodes/member.cpp nodes/array.cpp nodes/number.cpp
escape.cpp character.cpp string.cpp number.cpp integer.cpp fraction.cpp exp.cpp digit1_9.cpp 
member.cpp object.cpp array.cpp value.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_JSON)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection 
qor_workflow qor_pipeline qor_property qor_parser)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
