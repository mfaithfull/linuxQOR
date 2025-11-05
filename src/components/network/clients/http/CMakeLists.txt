cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_httpclient)

add_library(${target} qor_httpclient.cpp protohttpclient.cpp httpsocketclient.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_HTTPCLIENT)
target_link_libraries(${target} qor_http qor_module qor_host qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
