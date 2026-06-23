set(target visual)

add_executable(${target} visual.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")

target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

set(portable_libraries qor_module qor_compiler qor_host qor_datastructures qor_objectcontext
qor_thread qor_sync qor_application qor_role qor_error qor_log qor_desktopui
qor_logaggregator)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
set(os_libraries linqor_thread linqor_filesystem linqor_desktopui)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
set(os_libraries winqor_exebootstrap winqor_thread winqor_desktopui winqor_filesystem)
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