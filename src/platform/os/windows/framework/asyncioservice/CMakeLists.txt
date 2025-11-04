cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(winqor_asyncioservice SHARED winqor_asyncioservice.cpp asyncioservice.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines -fno-threadsafe-statics -fvisibility=hidden")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

target_compile_definitions(winqor_asyncioservice PRIVATE -DQOR_WINDOWSASYNCIOSERVICE)
target_link_libraries(winqor_asyncioservice qor_module qor_host qor_datastructures qor_objectcontext qor_injection qor_asyncioservice qor_error)