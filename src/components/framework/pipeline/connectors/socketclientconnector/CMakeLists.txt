cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_socketclientconnector)

add_library(${target} SHARED qor_socketclientconnector.cpp socketclientconnector.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_CLIENTSOCKETCONNECTOR)
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_thread qor_signals qor_pipeline qor_platform qor_sockets)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
