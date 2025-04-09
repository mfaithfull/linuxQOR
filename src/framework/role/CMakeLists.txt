cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_role SHARED qor_role.cpp role.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-threadsafe-statics -fvisibility=hidden")
target_compile_definitions(qor_role PUBLIC -DQOR_ROLE)
target_link_libraries(qor_role qor_module qor_thread qor_datastructures qor_objectcontext)