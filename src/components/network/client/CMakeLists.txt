cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_netclient)

add_library(${target} SHARED qor_netclient.cpp netclient.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_NETCLIENT)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection 
qor_log qor_error qor_pipeline qor_sockets qor_socketconnector)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
