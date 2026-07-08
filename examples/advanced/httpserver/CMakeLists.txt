set(target httpserver)

add_executable(${target} httpserverapp.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS 1)
SET(CMAKE_CXX_RESPONSE_FILE_LINK_FLAG "@")
SET(CMAKE_NINJA_FORCE_RESPONSE_FILE 1 CACHE INTERNAL "")

target_compile_definitions(${target} PRIVATE
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

set(portable_libraries qor_module qor_host qor_compiler qor_current
qor_thread qor_interception qor_injection qor_sync qor_issue qor_error qor_log qor_loghandler qor_logaggregator
qor_signals qor_pipeline qor_workflow qor_role qor_application qor_optionparser qor_filesystem qor_fileconnector
qor_task qor_platform qor_sockets qor_asyncioservice qor_socketconnector qor_http qor_parser qor_generator qor_netserver)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
set(os_libraries linqor_filesystem linqor_thread linqor_sockets linqor_asyncioservice)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
set(os_libraries winqor_filesystem winqor_thread winqor_process winqor_common winqor_asyncioservice winqor_sockets)
endif()

target_link_libraries(${target} ${portable_libraries} ${os_libraries})

if (WIN32)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
    COMMAND_EXPAND_LISTS
  )
endif ()
