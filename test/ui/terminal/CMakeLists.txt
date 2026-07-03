project(QORTerminalTests VERSION 1.0.0 DESCRIPTION "Querysoft Open Runtime Terminal Tests" LANGUAGES C CXX)
set(target QORTermTests)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(QORTermTests test_main.cpp
  primary.cpp
  tui.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")

target_compile_definitions(${target} PRIVATE CONSOLE
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

set(qor_libraries qor_compiler qor_module qor_host qor_reporting
qor_thread qor_interception qor_injection qor_sync qor_text qor_issue qor_error qor_log qor_signals 
qor_pipeline qor_workflow qor_role qor_application qor_optionparser qor_filesystem 
qor_log qor_loghandler qor_logaggregator
qor_task qor_threadmemory qor_platform qor_stdinsource qor_stdoutsink qor_console)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
target_link_libraries(${target} ${qor_libraries}
linqor_filesystem linqor_thread linqor_process uring linqor_console)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
target_link_libraries(${target} ${qor_libraries}
winqor_api_kernel winqor_api_user winqor_filesystem winqor_thread winqor_process winqor_console winqor_common) 
endif()

if (WIN32)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
    COMMAND_EXPAND_LISTS
  )
endif ()