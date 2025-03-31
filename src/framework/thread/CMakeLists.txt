cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_thread SHARED qor_thread.cpp thread.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-threadsafe-statics -fvisibility=hidden")
target_compile_definitions(qor_thread PUBLIC -DQOR_THREAD)
target_link_libraries(qor_thread qor_module qor_host)
