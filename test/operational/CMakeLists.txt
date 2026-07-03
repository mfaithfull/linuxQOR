project(QOROpTests VERSION 1.0.0 DESCRIPTION "Querysoft Open Runtime Operational Tests" LANGUAGES C CXX)
set(target QOROpTests)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(QOROpTests test_main.cpp
test_profiling.cpp
test_threadpool.cpp
)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")

target_compile_definitions(${target} PRIVATE CONSOLE
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

if(CMAKE_SYSTEM_NAME MATCHES Linux)
target_link_libraries(${target} qor_module qor_compiler qor_host 
qor_thread qor_interception qor_injection qor_sync qor_text qor_issue qor_error qor_log qor_text qor_signals 
qor_pipeline qor_workflow qor_role qor_application qor_optionparser qor_filesystem 
qor_task qor_threadmemory qor_platform qor_stdinsource qor_stdoutsink qor_property qor_console
linqor_filesystem linqor_thread linqor_process uring linqor_console qor_sockets linqor_sockets qor_loghandler qor_logaggregator
qor_stringsink )
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
target_link_libraries(${target} qor_module qor_compiler qor_host 
qor_thread qor_interception qor_injection qor_sync qor_text qor_issue qor_error qor_signals 
qor_pipeline qor_workflow qor_role qor_application qor_optionparser qor_filesystem 
qor_task qor_threadmemory qor_platform qor_stdinsource qor_stdoutsink qor_property qor_console
winqor_api_kernel winqor_api_winsock2 winqor_filesystem winqor_thread winqor_process winqor_console winqor_common qor_stdfstreamconnector 
qor_stdifstreamsource qor_stdofstreamsink qor_sockets qor_log qor_loghandler qor_logaggregator qor_http
qor_stringsource qor_base64encodefilter qor_stringsink qor_base64decodefilter winqor_sockets) 
endif()

if (WIN32)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
    COMMAND_EXPAND_LISTS
  )
endif ()