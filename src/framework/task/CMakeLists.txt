cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_task)

add_library(${target} SHARED qor_task.cpp
cancellation/cancellationregistration.cpp cancellation/cancellationsource.cpp
cancellation/cancellationstate.cpp cancellation/cancellationtoken.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_TASK)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
