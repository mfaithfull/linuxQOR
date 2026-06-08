set(target hydra)

add_executable(${target} hydra_main.cpp errorhandler.cpp logginghandler.cpp
workflow.cpp echoworkflow.cpp requestpipeline.cpp responsepipeline.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")

target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

set(portable_libraries 
qor_application qor_module qor_host qor_compiler qor_datastructures qor_objectcontext qor_sync
 qor_thread qor_platform qor_log qor_loghandler qor_logaggregator 
 qor_role qor_workflow qor_filesystem qor_pipeline
 qor_sockets qor_socketconnector
 qor_randomsource qor_stdoutsink)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    set(os_libraries linq_filesystem linq_thread linqor_sockets)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
    set(os_libraries winqor_filesystem winqor_thread winqor_sockets
    qor_property qor_optionparser qor_fileconnector qor_base64encodefilter
    qor_base64decodefilter)
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
