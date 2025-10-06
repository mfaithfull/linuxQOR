cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_error SHARED qor_error.cpp severityissue.cpp fatal.cpp serious.cpp
continuable.cpp warning.cpp note.cpp clibresult.cpp)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines -fno-threadsafe-statics -fvisibility=hidden")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()
target_compile_definitions(qor_issue PUBLIC -DQOR_ERROR)
target_link_libraries(qor_error qor_issue qor_module qor_host qor_issue qor_thread qor_interception)
