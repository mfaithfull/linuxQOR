cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

set(target testclient)

add_executable(${target} testclient_main.cpp)

target_compile_definitions(${target} PRIVATE -DQOR_PP_EXECUTABLE)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
target_link_libraries(${target} qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_thread qor_role qor_workflow qor_filesystem qor_optionparser qor_error qor_sockets qor_socketclient
qor_httpclient qor_pipeline qor_socketsink qor_socketsource qor_log
linqor_sockets linq_thread)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
target_link_libraries(${target} qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_thread qor_role qor_workflow qor_filesystem qor_optionparser qor_error qor_sockets qor_socketclient
qor_httpclient qor_pipeline qor_socketsink qor_socketsource qor_log)
endif()
