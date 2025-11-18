cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(target hydra)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(${target} hydra_main.cpp errorhandler.cpp logginghandler.cpp
workflow.cpp echoworkflow.cpp requestpipeline.cpp responsepipeline.cpp)

target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE)

set(portable_libraries 
qor_application qor_module qor_host qor_datastructures qor_objectcontext qor_sync
 qor_thread qor_platform qor_log qor_loghandler qor_logaggregator 
 qor_role qor_workflow qor_filesystem qor_pipeline
 qor_sockets qor_socketclientconnector qor_socketsink qor_socketsource
 qor_randomsource qor_stdoutsink)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    set(os_libraries linq_filesystem linq_thread linqor_sockets)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
    set(os_libraries winqor_filesystem winqor_thread winqor_sockets)
endif()

target_link_libraries(${target} ${portable_libraries} ${os_libraries})

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")