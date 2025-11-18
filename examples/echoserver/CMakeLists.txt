cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(target echoserver)
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(${target} echoserver_main.cpp echoserverapp.cpp serverworkflow.cpp clientsessionworkflow.cpp sessionpipeline.cpp
 errorhandler.cpp serverloghandler.cpp echorequestparser.cpp echorequestfilter.cpp)

target_compile_definitions(${target} PRIVATE -DQOR_PP_EXECUTABLE)

set(portable_libraries qor_module qor_host qor_compiler qor_datastructures qor_objectcontext
qor_platform qor_application qor_thread qor_role qor_workflow qor_filesystem qor_optionparser qor_pipeline qor_error qor_sockets
qor_asyncioservice qor_socketsessionconnector qor_socketsource qor_socketsink qor_log qor_logaggregator qor_loghandler qor_parser)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
set(os_libraries linqor_sockets linqor_asyncioservice linq_filesystem linq_thread)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
set(os_libraries winqor_sockets winqor_asyncioservice winq_filesystem winqor_thread)
endif()

target_link_libraries(${target} ${portable_libraries} ${os_libraries})

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")

