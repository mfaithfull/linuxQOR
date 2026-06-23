set(target server)

add_executable(${target} main.cpp server.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")

target_compile_definitions(${target} PRIVATE 
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

set(portable_libraries qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_thread qor_role qor_workflow qor_filesystem qor_optionparser qor_pipeline qor_error qor_sockets
qor_asyncioservice qor_socketconnector qor_log qor_logaggregator qor_loghandler 
qor_netserver qor_echo qor_http)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
set(os_libraries linqor_sockets linqor_asyncioservice linqor_filesystem linqor_thread)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
set(os_libraries winqor_sockets winqor_asyncioservice winqor_filesystem winqor_thread
qor_base64encodefilter qor_base64decodefilter qor_fileconnector qor_netclient)
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
