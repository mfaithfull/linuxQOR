cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(winq_thread SHARED winq_thread.cpp currentthread.cpp)
target_compile_definitions(winq_thread PUBLIC -DQOR_WINDOWSTHREAD)
target_link_libraries(winq_thread qor_module qor_host qor_datastructures qor_objectcontext qor_injection qor_thread qor_sync)