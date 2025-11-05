cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_error)

add_library(qor_error SHARED qor_error.cpp severityissue.cpp fatal.cpp serious.cpp
continuable.cpp warning.cpp note.cpp clibresult.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_ERROR)
target_link_libraries(${target} qor_issue qor_module qor_host qor_thread qor_interception)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")