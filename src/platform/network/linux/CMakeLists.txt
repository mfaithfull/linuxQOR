cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(linqor_sockets SHARED linqor_sockets.cpp sockets.cpp socket.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines -fno-threadsafe-statics -fvisibility=hidden")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

target_compile_definitions(linqor_sockets PRIVATE -DLINQOR_SOCKETS)
target_link_libraries(linqor_sockets qor_sockets qor_module qor_host qor_datastructures qor_objectcontext qor_sync qor_thread qor_platform)
