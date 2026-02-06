cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(target server)
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(${target} main.cpp server.cpp)

target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE)

set(portable_libraries qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_thread qor_role qor_workflow qor_filesystem qor_optionparser qor_pipeline qor_error qor_sockets
qor_asyncioservice qor_socketconnector qor_log qor_logaggregator qor_loghandler 
qor_netserver qor_echo)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
set(os_libraries linqor_sockets linqor_asyncioservice linq_filesystem linq_thread)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
set(os_libraries winqor_sockets winqor_asyncioservice winqor_filesystem winqor_thread
qor_base64encodefilter qor_base64decodefilter qor_fileconnector qor_netclient)
endif()

target_link_libraries(${target} ${portable_libraries} ${os_libraries})

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")

if (WIN32)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
    COMMAND_EXPAND_LISTS
  )
endif ()