cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

set(target visual)

add_executable(${target} visual.cpp)

target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE)

set(portable_libraries qor_compiler qor_host qor_datastructures qor_objectcontext
qor_thread qor_sync qor_application qor_role qor_error qor_log qor_desktopui
qor_logaggregator)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
set(os_libraries linq_thread)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
set(os_libraries winqor_exebootstrap winqor_thread winqor_desktopui winqor_filesystem)
endif()

target_link_libraries(${target} ${portable_libraries} ${os_libraries})

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")

if (WIN32)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
    COMMAND_EXPAND_LISTS
  )
endif ()