cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_loghandler)

add_library(${target} SHARED qor_loghandler.cpp
loghandler.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_LOGHANDLER)
target_link_libraries(${target} qor_module qor_host qor_thread qor_sync qor_signals qor_log)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
