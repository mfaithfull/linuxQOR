cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_arch)

add_library(${target} SHARED qor_architecture.cpp cpu.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_PP_ARCH)
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(${target} qor_module qor_host qor_thread winqor_sharedbootstrap)
endif()
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(${target} qor_module qor_host qor_thread)
endif()

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")