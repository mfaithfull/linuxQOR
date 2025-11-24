cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_filesystem)

add_library(${target} SHARED qor_filesystem.cpp filesystem.cpp path.cpp root.cpp fileindex.cpp folder.cpp 
file/file.cpp file/filereader.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_FILESYSTEM)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_sync qor_thread qor_platform qor_error)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")