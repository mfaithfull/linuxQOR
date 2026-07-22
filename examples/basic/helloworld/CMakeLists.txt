project(HelloWorld VERSION 1.0.0 DESCRIPTION "Hello World QOR Example" LANGUAGES CXX)
set(target helloworld)
add_executable(${target} main.cpp )
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
target_compile_definitions(${target} PRIVATE qor_pp_ver_project=${PROJECT_VERSION})
target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform qor_application)

if (WIN32)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
    COMMAND_EXPAND_LISTS
  )
endif ()
