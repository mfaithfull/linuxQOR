set(target QORMPL)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(${target} test_main.cpp)

target_compile_definitions(${target} PRIVATE CONSOLE 
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

set(qor_libraries qor_module qor_compiler qor_host)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
target_link_libraries(${target} ${qor_libraries})
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
target_link_libraries(${target} ${qor_libraries}) 
endif()

if (WIN32)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
    COMMAND_EXPAND_LISTS
  )
endif ()
