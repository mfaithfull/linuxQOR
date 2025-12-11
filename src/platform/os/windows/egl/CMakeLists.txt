cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_egl)

add_library(${target} SHARED winqor_egl.cpp egl.cpp display.cpp context.cpp window.cpp)

target_include_directories(${target} PRIVATE "F:/Develop/Native_SDK/include/")

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINEGL)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_error qor_issue qor_thread 
qor_interception qor_egl winqor_api_kernel winqor_api_user winqor_common winqor_sharedbootstrap winqor_gui
"F:/Develop/Native_SDK/lib/Windows_x86_64/libEGL.lib")

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")