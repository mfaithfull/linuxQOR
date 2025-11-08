cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(target fileprocessor)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(${target} fileprocessor_main.cpp fileprocessorapp.cpp fileprocessorworkflow.cpp)

target_compile_definitions(${target} PRIVATE -DQOR_PP_EXECUTABLE)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    target_link_libraries(${target} qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_thread qor_role qor_workflow qor_filesystem qor_optionparser qor_pipeline linq_filesystem 
qor_error qor_parser)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
    target_link_libraries(${target} qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_thread qor_role qor_workflow qor_filesystem qor_optionparser qor_pipeline winqor_filesystem 
qor_error qor_parser)
endif()

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")