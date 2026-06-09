set(target linebyline)

add_executable(${target} linebyline_main.cpp linebylineworkflow.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")

target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    target_link_libraries(${target} qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_thread qor_role qor_workflow qor_filesystem qor_optionparser qor_pipeline 
linqor_filesystem linqor_thread
qor_error qor_linereader qor_stdoutsink qor_fileconnector)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
    target_link_libraries(${target} qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_thread qor_role qor_workflow qor_filesystem qor_optionparser qor_pipeline 
winqor_filesystem winqor_thread
qor_error qor_linereader qor_stdoutsink qor_fileconnector)
endif()

if (WIN32)
  add_custom_command(
    TARGET linebyline POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:linebyline> $<TARGET_FILE_DIR:linebyline>
    COMMAND_EXPAND_LISTS
  )
endif ()
