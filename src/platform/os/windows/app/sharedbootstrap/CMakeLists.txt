project(QOR VERSION 0.5.0 DESCRIPTION "QOR Windows DLL Bootstrap Module" LANGUAGES CXX)
set(target winqor_sharedbootstrap)

add_library(winqor_sharedbootstrap winqsharedbootstrap.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_link_libraries(${target})
