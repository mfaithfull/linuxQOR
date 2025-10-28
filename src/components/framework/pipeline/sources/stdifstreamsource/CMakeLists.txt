cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_stdifstreamsource SHARED qor_stdifstreamsource.cpp stdifstreamsource.cpp stdifstreamreader.cpp)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines -fno-threadsafe-statics -fvisibility=hidden")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

target_compile_definitions(qor_stdifstreamsource PRIVATE -DQOR_STDIFSTREAMSOURCE)
target_link_libraries(qor_stdifstreamsource qor_module qor_datastructures qor_objectcontext qor_signals qor_pipeline 
qor_filesystem qor_stdfstreamconnector)