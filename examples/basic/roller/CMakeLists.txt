set(target roller)

add_executable(${target} main.cpp customrole.cpp customfeature.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")

target_compile_definitions(${target} PRIVATE 
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

if(CMAKE_SYSTEM_NAME MATCHES Linux)
set(plaformlibs linqor_thread)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
set(plaformlibs winqor_thread)
endif()

target_link_libraries(${target} ${plaformlibs} qor_module qor_host qor_compiler qor_current qor_platform qor_application qor_role qor_log qor_loghandler qor_error)

if (WIN32)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
    COMMAND_EXPAND_LISTS
  )
endif ()
