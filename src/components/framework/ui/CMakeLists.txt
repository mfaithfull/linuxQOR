cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_ui)

add_library(${target} SHARED qor_ui.cpp visualworkflow.cpp 
frameworkproxies/applicationproxy.cpp
frameworkproxies/roleproxy.cpp
applicationmodel.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_UI)
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_role qor_application qor_workflow qor_signals qor_error
qor_property)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")

add_subdirectory(egl)
#add_subdirectory(cairo)
add_subdirectory(opengles)
add_subdirectory(renderer)