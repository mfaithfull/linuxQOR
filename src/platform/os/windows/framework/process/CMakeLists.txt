cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(winq_process SHARED winq_process.cpp currentprocess.cpp)
target_compile_definitions(winq_process PRIVATE -DQOR_WINDOWSPROCESS)
target_link_libraries(winq_process qor_module qor_host qor_datastructures qor_objectcontext qor_injection qor_thread qor_issue qor_error)
