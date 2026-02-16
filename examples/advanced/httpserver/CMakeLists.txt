cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

set(target httpserver)

add_executable(${target} httpserver_main.cpp app.cpp)

target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE)

set(portable_libraries qor_compiler qor_host qor_datastructures qor_objectcontext 
qor_thread qor_interception qor_injection qor_sync qor_issue qor_error qor_log qor_loghandler qor_logaggregator
qor_signals qor_pipeline qor_workflow qor_role qor_application qor_optionparser qor_filesystem qor_fileconnector
qor_task qor_threadmemory qor_platform qor_stdinsource qor_stdoutsink qor_property qor_console
qor_sockets qor_asyncioservice qor_socketconnector qor_http qor_parser qor_generator qor_netserver)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
set(os_libraries linqor_sockets linqor_asyncioservice linq_filesystem linq_thread)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
set(os_libraries winqor_filesystem winqor_thread winqor_process winqor_common winqor_asyncioservice winqor_sockets)
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