project(Client VERSION 1.0.0 LANGUAGES CXX)
set(target client)
add_executable(${target} main.cpp workflow.cpp)
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
target_compile_definitions(${target} PRIVATE 
qor_pp_ver_major=${PROJECT_VERSION_MAJOR} 
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

set(qor_libraries qor_module qor_host qor_compiler 
qor_platform qor_application qor_current qor_thread qor_role qor_workflow qor_optionparser qor_pipeline qor_error qor_sockets
qor_console qor_socketconnector qor_stdinsource qor_stdoutsink qor_parser qor_log qor_logaggregator qor_loghandler
qor_echo qor_netclient qor_fileconnector)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
target_link_libraries(${target} ${qor_libraries}
linqor_sockets linqor_console linqor_thread linqor_filesystem)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
target_link_libraries(${target} ${qor_libraries}
winqor_sockets winqor_console winqor_thread winqor_filesystem)
endif()

if (WIN32)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
    COMMAND_EXPAND_LISTS
  )
endif ()
