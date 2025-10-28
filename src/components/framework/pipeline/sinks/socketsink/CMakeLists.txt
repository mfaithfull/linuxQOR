cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_socketsink SHARED qor_socketsink.cpp socketsink.cpp socketwriter.cpp)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines -fno-threadsafe-statics -fvisibility=hidden")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

target_compile_definitions(qor_socketsink PRIVATE -DQOR_SOCKETSINK)
target_link_libraries(qor_socketsink qor_module qor_datastructures qor_objectcontext qor_signals qor_pipeline 
qor_platform qor_sockets qor_socketclientconnector)