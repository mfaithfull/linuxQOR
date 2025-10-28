cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(linq_process SHARED linq_process.cpp currentprocess.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-threadsafe-statics -fvisibility=hidden")
target_compile_definitions(linq_process PRIVATE -DQOR_LINUXPROCESS)
target_link_libraries(linq_process qor_module qor_host qor_datastructures qor_objectcontext qor_injection)