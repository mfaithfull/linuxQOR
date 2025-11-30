cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(linqor_asyncioservice SHARED linqor_asyncioservice.cpp asyncioservice.cpp 
iouringservice/iouring.cpp iouringservice/iouringeventprocessor.cpp iouringservice/iouringinitiator.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(linqor_asyncioservice PRIVATE QOR_LINUXASYNCIOSERVICE)
target_link_libraries(linqor_asyncioservice qor_module qor_host qor_datastructures qor_objectcontext qor_injection qor_asyncioservice qor_error qor_log uring rt)