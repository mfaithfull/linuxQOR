cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(linq_filesystem SHARED linq_filesystem.cpp filesystem.cpp filestat.cpp file.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-threadsafe-statics -fvisibility=hidden")
target_compile_definitions(linq_filesystem PRIVATE -DQOR_LINUXFILESYSTEM)
target_link_libraries(linq_filesystem qor_module qor_host qor_datastructures qor_objectcontext qor_injection qor_filesystem rt)