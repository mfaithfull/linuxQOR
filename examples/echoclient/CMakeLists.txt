cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(target echoclient)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(${target} echoclient_main.cpp echoclientworkflow.cpp echorequestpipeline.cpp echoresponsepipeline.cpp echoresponsefilter.cpp
echoresponseparser.cpp echoclienterrorhandler.cpp echoclientloghandler.cpp)

target_compile_definitions(${target} PRIVATE -DQOR_PP_EXECUTABLE)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
target_link_libraries(${target} qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_thread qor_role qor_workflow qor_optionparser qor_pipeline qor_error qor_sockets
qor_console qor_socketconnector qor_stdinsource qor_stdoutsink qor_parser qor_log
qor_logaggregator qor_loghandler
linqor_sockets linq_console linq_thread linq_filesystem)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
target_link_libraries(${target} qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_thread qor_role qor_workflow qor_optionparser qor_pipeline qor_error qor_sockets
qor_console qor_socketconnector qor_stdinsource qor_stdoutsink qor_parser qor_log
qor_logaggregator qor_loghandler
winqor_sockets winqor_console winqor_thread winq_filesystem)
endif()

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")