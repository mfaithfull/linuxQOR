cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_sockets SHARED qor_sockets.cpp sockets.cpp socket.cpp address.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines -fno-threadsafe-statics -fvisibility=hidden")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

target_compile_definitions(qor_sockets PRIVATE -DQOR_SOCKETS)
set(portable_libraries qor_module qor_host qor_datastructures qor_objectcontext qor_sync qor_thread qor_platform)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
set(os_libraries)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
set(os_libraries ws2_32)
endif()

target_link_libraries(qor_sockets ${portable_libraries} ${os_libraries})

add_subdirectory(linux)
add_subdirectory(windows)