cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

set(target webserver)

add_executable(${target} webserver_main.cpp)

target_compile_definitions(${target} PRIVATE -DQOR_PP_EXECUTABLE)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
target_link_libraries(${target} qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_application qor_thread qor_role qor_workflow qor_platform qor_sockets linqor_sockets)
# qor_interception qor_injection qor_sync qor_issue qor_error qor_signals 
#qor_pipeline   qor_optionparser 
#qor_filesystem qor_task qor_threadmemory qor_platform qor_property 
#linq_filesystem linq_thread linq_process uring linq_console)
endif()

#if(CMAKE_SYSTEM_NAME MATCHES Windows)
#target_link_libraries(${target} qor_compiler qor_host qor_datastructures qor_objectcontext 
#qor_thread qor_interception qor_injection qor_sync qor_issue qor_error qor_signals 
#qor_pipeline qor_workflow qor_role qor_application qor_optionparser qor_filesystem 
#qor_task qor_threadmemory qor_platform qor_stdinsource qor_stdoutsink qor_property qor_console
#winqor_winapi winqor_filesystem winqor_thread winqor_process winqor_console winqor_common)
#endif()
