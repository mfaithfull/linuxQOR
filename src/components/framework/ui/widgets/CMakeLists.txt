cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_widgets)

add_library(${target} SHARED qor_widgets.cpp widgetbase.cpp panel.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WIDGETS)
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_role qor_application qor_error qor_layout)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
