cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(winq_filesystem SHARED winq_filesystem.cpp filesystem.cpp file.cpp)
target_compile_definitions(winq_filesystem PRIVATE -DQOR_WINDOWSFILESYSTEM)
target_link_libraries(winq_filesystem qor_module qor_host qor_datastructures qor_objectcontext qor_injection qor_filesystem winqsharedbootstrap)
