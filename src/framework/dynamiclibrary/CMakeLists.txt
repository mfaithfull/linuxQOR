cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_dynamiclibrary)

add_library(${target} SHARED qor_dynamiclibrary.cpp
dynamiclibrary.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_DYNAMICLIBARY)
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(${target} qor_module qor_objectcontext qor_thread winqor_sharedbootstrap winqor_thread)
endif()
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(${target} qor_module qor_objectcontext qor_thread linqor_thread)
endif()

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")