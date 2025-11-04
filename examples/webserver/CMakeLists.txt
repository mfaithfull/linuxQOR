cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines -fno-threadsafe-statics -fvisibility=hidden")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

add_executable(webserver webserver_main.cpp)

target_compile_definitions(webserver PRIVATE -DQOR_PP_EXECUTABLE)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
target_link_libraries(webserver qor_module qor_host QORCompiler qor_datastructures qor_objectcontext
qor_application qor_thread qor_role qor_workflow qor_platform qor_sockets linqor_sockets)
# qor_interception qor_injection qor_sync qor_issue qor_error qor_signals 
#qor_pipeline   qor_optionparser 
#qor_filesystem qor_task qor_threadmemory qor_platform qor_property 
#linq_filesystem linq_thread linq_process uring linq_console)
endif()

#if(CMAKE_SYSTEM_NAME MATCHES Windows)
#target_link_libraries(webserver QORCompiler qor_host qor_datastructures qor_objectcontext 
#qor_thread qor_interception qor_injection qor_sync qor_issue qor_error qor_signals 
#qor_pipeline qor_workflow qor_role qor_application qor_optionparser qor_filesystem 
#qor_task qor_threadmemory qor_platform qor_stdinsource qor_stdoutsink qor_property qor_console
#qor_winapi winqor_filesystem winq_thread winq_process winq_console winq_common)
#endif()
