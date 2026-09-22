set(target QORTestTEF)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(${target} test_main.cpp
  test_tef.cpp
  test_type_traits.cpp
  test_type_erased_object.cpp
  test_type_erased_function.cpp
  test_common.cpp
  test_member_function.cpp
  test_make_function.cpp
  test_function_traits.cpp
  test_function_cast.cpp
  test_allocator.cpp
  test_function.cpp)

target_compile_definitions(${target} PRIVATE CONSOLE 
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

set(qor_libraries qor_module qor_compiler qor_host qor_arch qor_current qor_thread
qor_injection qor_sync qor_text qor_issue qor_error qor_log qor_reporting qor_platform
qor_application qor_role qor_tef)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
target_link_libraries(${target} ${qor_libraries} linqor_filesystem linqor_thread 
linqor_process linqor_console)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
target_link_libraries(${target} ${qor_libraries} winqor_api_kernel winqor_filesystem 
winqor_thread winqor_process winqor_console winqor_common) 
endif()

if (WIN32)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
    COMMAND_EXPAND_LISTS
  )
endif ()
