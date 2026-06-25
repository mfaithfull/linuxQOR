set(target QORTestFileSystem)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(${target} test_main.cpp
  smoke.cpp )

target_compile_definitions(${target} PRIVATE CONSOLE 
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

set(qor_libraries qor_module qor_compiler qor_host qor_datastructures qor_objectcontext qor_arch qor_thread qor_current
qor_injection qor_sync qor_text qor_issue qor_error qor_log qor_reporting qor_pipeline qor_platform
qor_loghandler qor_logaggregator qor_filesystem qor_asyncioservice qor_application qor_role qor_optionparser)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
target_link_libraries(${target} ${qor_libraries} linqor_filesystem linqor_thread 
linqor_process uring linqor_console linqor_asyncioservice)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
target_link_libraries(${target} ${qor_libraries} winqor_api_kernel winqor_filesystem 
winqor_thread winqor_process winqor_console winqor_common winqor_asyncioservice) 
endif()

if (WIN32)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
    COMMAND_EXPAND_LISTS
  )
endif ()
