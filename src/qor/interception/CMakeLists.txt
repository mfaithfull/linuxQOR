cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_interception SHARED qor_interception.cpp callcontext.cpp functioncontext.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-threadsafe-statics -fvisibility=hidden")
target_compile_definitions(qor_interception PUBLIC -DQOR_INTERCEPTION)
target_link_libraries(qor_interception qor_module qor_host qor_objectcontext qor_thread)
