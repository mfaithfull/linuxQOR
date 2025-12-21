cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_layout)

add_library(${target} SHARED qor_layout.cpp box.cpp node.cpp boxhelper.cpp vbox.cpp hbox.cpp size.cpp nodedecorator.cpp
flexboxhelper.cpp flexboxconfig.cpp flexbox.cpp dbox.cpp gridbox.cpp reflect.cpp operators.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_LAYOUT)
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_role qor_application qor_error)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
