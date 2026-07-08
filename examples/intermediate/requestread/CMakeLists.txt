set(target requestread)

add_executable(${target} main.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")

target_compile_definitions(${target} PRIVATE 
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

set(qorlibraries qor_module qor_host qor_current qor_compiler qor_platform 
qor_application qor_filesystem qor_pipeline qor_error qor_fileconnector 
qor_http qor_parsersink qor_parser qor_loghandler qor_log qor_error
qor_issue)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    target_link_libraries(${target} ${qorlibraries} linqor_filesystem linqor_thread)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
    target_link_libraries(${target} ${qorlibraries} winqor_filesystem winqor_thread)
endif()

if (WIN32)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
    COMMAND_EXPAND_LISTS
  )
endif ()
