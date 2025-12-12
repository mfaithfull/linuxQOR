cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_opengles)

add_library(${target} SHARED winqor_opengles.cpp opengles.cpp)

target_include_directories(${target} PRIVATE "F:/Develop/Native_SDK/include/")

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINOPENGLES)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_error qor_issue qor_thread 
qor_interception qor_egl qor_opengles winqor_api_kernel winqor_api_user winqor_common winqor_sharedbootstrap winqor_gui 
"F:/Develop/Native_SDK/lib/Windows_x86_64/libGLESv2.lib")

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")