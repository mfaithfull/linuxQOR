cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_log)

add_library(qor_log SHARED qor_log.cpp log.cpp debug.cpp important.cpp
impactful.cpp informative.cpp imperative.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_LOG)
target_link_libraries(${target} qor_issue qor_module qor_host qor_thread qor_interception)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")