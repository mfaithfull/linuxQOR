cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(target requestread)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(${target} main.cpp)

target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    target_link_libraries(${target} qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_filesystem qor_pipeline qor_error qor_fileconnector 
qor_http qor_parsersink qor_parser
linq_filesystem)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
    target_link_libraries(${target} qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_filesystem qor_pipeline qor_error qor_fileconnector 
qor_http qor_parsersink qor_parser qor_asyncioservice qor_sockets qor_socketconnector qor_json qor_base64encodefilter
qor_base64decodefilter qor_optionparser qor_stdoutsink qor_loghandler qor_log
winqor_asyncioservice winqor_filesystem winqor_thread)
endif()

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")

if (WIN32)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
    COMMAND_EXPAND_LISTS
  )
endif ()