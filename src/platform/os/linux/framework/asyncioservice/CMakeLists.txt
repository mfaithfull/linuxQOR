cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(linqor_asyncioservice SHARED linqor_asyncioservice.cpp asyncioservice.cpp 
iouringservice/iouringservice.cpp iouringservice/iouring.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-threadsafe-statics -fvisibility=hidden")

target_compile_definitions(linqor_asyncioservice PRIVATE -DQOR_LINUXASYNCIOSERVICE)
target_link_libraries(linqor_asyncioservice qor_module qor_host qor_datastructures qor_objectcontext qor_injection qor_asyncioservice qor_error uring rt)