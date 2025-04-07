cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_objectcontext SHARED qor_objectcontext.cpp anyobject.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-threadsafe-statics -fvisibility=hidden")
target_compile_definitions(qor_objectcontext PUBLIC -DQOR_OBJECTCONTEXT)
target_link_libraries(qor_objectcontext qor_module qor_host)
