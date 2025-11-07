cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_sockets)

add_library(${target} SHARED winqor_sockets.cpp sockets.cpp socket.cpp "addresshelpers.h")

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DWINQOR_SOCKETS)
target_link_libraries(${target} qor_sockets qor_module qor_host qor_datastructures qor_objectcontext qor_sync qor_thread qor_platform
winqor_asyncioservice winqor_api_winsock2)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")