cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(target parallel)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(${target} main.cpp)

target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE qor_pp_ver_major=1 )

if(CMAKE_SYSTEM_NAME MATCHES Linux)
target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform qor_application qor_role qor_thread qor_signals qor_task 
qor_log qor_loghandler qor_logaggregator qor_filesystem
linq_thread linq_filesystem)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform qor_application qor_role qor_thread qor_signals qor_task 
qor_log qor_loghandler qor_logaggregator qor_filesystem qor_sockets qor_asyncioservice qor_pipeline qor_base64encodefilter 
qor_base64decodefilter qor_optionparser qor_socketconnector qor_property qor_fileconnector qor_stdoutsink
winqor_thread winqor_filesystem)
endif()

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
