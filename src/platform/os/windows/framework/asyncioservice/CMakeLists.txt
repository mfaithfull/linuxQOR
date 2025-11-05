cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_asyncioservice)

add_library(${target} SHARED winqor_asyncioservice.cpp asyncioservice.cpp "iocpinitiator.h" "iocpeventprocessor.h" "readop.h" "iocpawaiter.h" "listenop.h" "bindop.h" "acceptop.h" "sendop.h" "iocpinitiator.cpp" "iocpeventprocessor.cpp" "overlappedoperationcancellable.h" "overlappedoperationbase.h" "iostate.h" "overlapped.h" "overlappedoperation.h" "socketacceptimpl.cpp" "connectop.h" "socketconnectimpl.cpp" "disconnectop.h" "socketdisconnectimpl.cpp" "recvfromop.h" "socketrecvfromimpl.cpp" "recvop.h" "socketrecvimpl.cpp" "socketsendimpl.cpp" "sendtoop.h" "socketsendtoimpl.cpp" "wsabuffer.h")

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_WINDOWSASYNCIOSERVICE)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_injection 
qor_asyncioservice qor_error qor_sockets Mswsock)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")