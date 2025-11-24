cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(target echoclient2)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(${target} echoclient2_main.cpp echoclient2workflow.cpp)

target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
target_link_libraries(${target} qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_thread qor_role qor_workflow qor_optionparser qor_pipeline qor_error qor_sockets
qor_console qor_socketclientconnector qor_stdinsource qor_stdoutsink qor_socketsource qor_socketsink qor_parser qor_log
qor_logaggregator qor_loghandler qor_echo qor_netclient
linqor_sockets linq_console linq_thread linq_filesystem)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
target_link_libraries(${target} qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_thread qor_role qor_workflow qor_optionparser qor_pipeline qor_error qor_sockets
qor_console qor_socketclientconnector qor_stdinsource qor_stdoutsink qor_socketsource qor_socketsink qor_parser qor_log
qor_logaggregator qor_loghandler qor_echo qor_netclient
winqor_sockets winqor_console winq_thread winq_filesystem)
endif()

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")