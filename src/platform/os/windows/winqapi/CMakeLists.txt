cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_winapi SHARED qor_winapi.cpp kernel/kconsole.cpp  kernel/kerrorhandling.cpp kernel/kdll.cpp "kernel/khandle.cpp")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines -fno-threadsafe-statics -fvisibility=hidden")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

target_compile_definitions(qor_winapi PUBLIC -DQOR_WINAPI)
target_link_libraries(qor_winapi qor_module qor_datastructures qor_objectcontext qor_error qor_issue)