cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_api_kernel)

add_library(${target} SHARED winqor_api_kernel.cpp kconsole.cpp  kerrorhandling.cpp kdll.cpp khandle.cpp kio.cpp sync.cpp kfile.cpp
errorhandler.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_WINAPI PUBLIC ${global_defines})
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_error qor_issue)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")