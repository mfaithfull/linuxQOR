cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(target linebyline)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(${target} linebyline_main.cpp linebylineworkflow.cpp)

target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    target_link_libraries(${target} qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_thread qor_role qor_workflow qor_filesystem qor_optionparser qor_pipeline 
linq_filesystem linq_thread
qor_error qor_linereader qor_stdoutsink qor_fileconnector)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
    target_link_libraries(${target} qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_thread qor_role qor_workflow qor_filesystem qor_optionparser qor_pipeline 
winqor_filesystem winqor_thread
qor_error qor_linereader qor_stdoutsink qor_fileconnector)
endif()

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")

if (WIN32)
  add_custom_command(
    TARGET linebyline POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:linebyline> $<TARGET_FILE_DIR:linebyline>
    COMMAND_EXPAND_LISTS
  )
endif ()