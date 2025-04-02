cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_injection SHARED qor_injection.cpp typeregistry.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-threadsafe-statics -fvisibility=hidden")
target_compile_definitions(qor_injection PUBLIC -DQOR_INJECTION)
target_link_libraries(qor_injection qor_module qor_host qor_datastructures qor_objectcontext)
