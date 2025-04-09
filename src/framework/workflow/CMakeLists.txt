cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_workflow SHARED qor_workflow.cpp workflow.cpp compound_workflow.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-threadsafe-statics -fvisibility=hidden")
target_compile_definitions(qor_workflow PUBLIC -DQOR_WORKFLOW)
target_link_libraries(qor_workflow qor_module qor_thread qor_datastructures qor_objectcontext)