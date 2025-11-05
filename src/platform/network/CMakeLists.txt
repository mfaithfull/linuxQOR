cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_sockets)

add_library(${target} SHARED qor_sockets.cpp sockets.cpp socket.cpp address.cpp "ipaddress.h" "ipv4address.h" "ipv6address.h" "ipendpoint.h" "ipv4endpoint.h" "ipv6endpoint.h" "ipv4address.cpp" "ipv6address.cpp" "ipv4endpoint.cpp" "ipaddress.cpp" "ipendpoint.cpp" "ipv6endpoint.cpp")

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_SOCKETS)
set(portable_libraries qor_module qor_host qor_datastructures qor_objectcontext qor_sync qor_thread qor_platform)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    set(os_libraries)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
    set(os_libraries ws2_32)
endif()

target_link_libraries(${target} ${portable_libraries} ${os_libraries})

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")

add_subdirectory(linux)
add_subdirectory(windows)