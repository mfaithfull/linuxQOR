set(target echoclient)

add_executable(${target} echoclient_main.cpp echoclientworkflow.cpp echorequestpipeline.cpp echoresponsepipeline.cpp echoresponsefilter.cpp
echoresponseparser.cpp echoclienterrorhandler.cpp echoclientloghandler.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")

target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

if(CMAKE_SYSTEM_NAME MATCHES Linux)
target_link_libraries(${target} qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_thread qor_role qor_workflow qor_optionparser qor_pipeline qor_error qor_sockets
qor_console qor_socketconnector qor_stdinsource qor_stdoutsink qor_parser qor_log
qor_logaggregator qor_loghandler
linqor_sockets linq_console linq_thread linq_filesystem)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
target_link_libraries(${target} qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_thread qor_role qor_workflow qor_optionparser qor_pipeline qor_error qor_sockets
qor_console qor_socketconnector qor_stdinsource qor_stdoutsink qor_parser qor_log
qor_logaggregator qor_loghandler
winqor_sockets winqor_console winqor_thread winqor_filesystem)
endif()

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")

if (WIN32)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
    COMMAND_EXPAND_LISTS
  )
endif ()