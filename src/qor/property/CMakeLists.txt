cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_property)

add_library(${target} SHARED qor_property.cpp propertyvalue.cpp "propertyvaluebase.h" "propertybase.h" "propertygenerator.h")

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_PROPERTY)
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
