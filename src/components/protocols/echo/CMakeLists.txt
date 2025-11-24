cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_echo)

add_library(${target} SHARED qor_echo.cpp echoprotocol.cpp
echorequestsource.cpp echoresponsefilter.cpp echoresponseparser.cpp
echoresponsesink.cpp echoservicefilter.cpp echorequestparser.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_HTTP)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection 
qor_parser qor_stdinsource qor_stdoutsink)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")