cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_com)

add_library(${target} SHARED winqor_com.cpp
server/baseparts/coobjectbase.cpp
server/baseparts/unknownimplbase.cpp
server/baseparts/serverobjectbase.cpp
client/clientbase.cpp
server/module/classfactory.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINCOM PUBLIC ${global_defines})
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_error qor_issue winqor_api_ole32 winqor_api_user winqor_api_kernel)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
