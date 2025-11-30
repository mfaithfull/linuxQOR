cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(linqor_sockets SHARED linqor_sockets.cpp sockets.cpp socket.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(linqor_sockets PRIVATE -DLINQOR_SOCKETS)
target_link_libraries(linqor_sockets qor_sockets qor_module qor_host qor_datastructures qor_objectcontext qor_sync qor_thread qor_platform
linqor_asyncioservice)
