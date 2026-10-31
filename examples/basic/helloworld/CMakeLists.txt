set(target helloworld)
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
add_executable(${target} main.cpp )
include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform qor_application)

if (WIN32)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
    COMMAND_EXPAND_LISTS
  )
endif ()
