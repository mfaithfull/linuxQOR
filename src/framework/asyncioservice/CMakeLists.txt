cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_asyncioservice)

add_library(${target} SHARED qor_asyncioservice.cpp asyncioservice.cpp asynciocontext.cpp 
asyncioeventprocessor.cpp asyncioinitiator.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_ASYNCIOSERVICE)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_sync qor_thread
qor_task qor_role)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")