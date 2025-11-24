cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(target roller)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(${target} main.cpp app.cpp customrole.cpp)

target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE qor_pp_ver_major=1 )

if(CMAKE_SYSTEM_NAME MATCHES Linux)
target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform qor_application qor_role qor_thread
linq_thread)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform qor_application qor_role qor_thread
winqor_thread)
endif()

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
