cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_asyncioservice)

add_library(${target} SHARED winqor_asyncioservice.cpp asyncioservice.cpp iocpinitiator.cpp iocpeventprocessor.cpp socketacceptimpl.cpp
socketconnectimpl.cpp socketdisconnectimpl.cpp socketrecvfromimpl.cpp
socketrecvimpl.cpp socketsendimpl.cpp sendtoop.h socketsendtoimpl.cpp ioservice.cpp scheduleoperation.cpp
timedscheduleoperation.cpp timerqueue.cpp timerthreadstate.cpp iocompletionport.cpp waitabletimerevent.cpp autoresetevent.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINDOWSASYNCIOSERVICE)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_injection 
qor_asyncioservice qor_error qor_sockets winqor_common winqor_api_winsock2)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")