cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winq_common)

add_library(${target} SHARED winq_common.cpp handles/handle.cpp "constants.h" "handles/typedhandle.h")

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_WINCOMMON)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_error qor_issue qor_thread qor_interception qor_winapi winqsharedbootstrap)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")