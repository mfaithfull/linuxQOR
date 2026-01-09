cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(target eglantine)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(${target} main.cpp eltkeycontroller.cpp eltpointercontroller.cpp eltsession.cpp)

target_include_directories(${target} PRIVATE)

target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE qor_pp_ver_major=1 )

target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform qor_application linqor_waylandclient linqor_xdgshell linqor_waylandegl
linqor_egl linqor_opengles
wayland-egl wayland-client EGL GL)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
