cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_api_uxtheme)

add_library(${target} SHARED winqor_api_uxtheme.cpp 
uxtheme.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINAPIUXTHEME PUBLIC ${global_defines})
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_error qor_issue winqor_api_kernel uxtheme)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
