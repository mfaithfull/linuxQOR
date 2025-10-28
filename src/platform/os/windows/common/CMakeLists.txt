cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(winq_common SHARED winq_common.cpp handles/handle.cpp "constants.h" "handles/typedhandle.h")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines -fno-threadsafe-statics -fvisibility=hidden")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

target_compile_definitions(winq_common PRIVATE -DQOR_WINCOMMON)
target_link_libraries(winq_common qor_module qor_host qor_datastructures qor_objectcontext qor_error qor_issue qor_thread qor_interception qor_winapi winqsharedbootstrap)