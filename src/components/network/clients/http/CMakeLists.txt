cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_httpclient SHARED qor_httpclient.cpp protohttpclient.cpp httpsocketclient.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines -fno-threadsafe-statics -fvisibility=hidden")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

target_compile_definitions(qor_httpclient PRIVATE -DQOR_HTTPCLIENT)
target_link_libraries(qor_httpclient qor_http qor_module qor_host qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection)