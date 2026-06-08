set(target parallel)

add_executable(${target} main.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")

target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

if(CMAKE_SYSTEM_NAME MATCHES Linux)
target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform qor_application qor_role qor_thread qor_signals qor_task 
qor_log qor_loghandler qor_logaggregator qor_filesystem
linq_thread linq_filesystem)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform qor_application qor_role qor_thread qor_signals qor_task 
qor_log qor_loghandler qor_logaggregator qor_filesystem qor_sockets qor_asyncioservice qor_pipeline qor_base64encodefilter 
qor_base64decodefilter qor_optionparser qor_socketconnector qor_property qor_fileconnector qor_stdoutsink
winqor_thread winqor_filesystem)
endif()

if (WIN32)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
    COMMAND_EXPAND_LISTS
  )
endif ()
