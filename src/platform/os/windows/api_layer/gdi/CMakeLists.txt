cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_api_gdi)

add_library(${target} SHARED winqor_api_gdi.cpp 
bitmap.cpp
brush.cpp
clipping.cpp
colour.cpp
coordtransform.cpp
devicecontext.cpp
drawing.cpp
text.cpp
pen.cpp
painting.cpp
path.cpp
printing.cpp
region.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINAPIGDI PUBLIC ${global_defines})
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_error qor_issue winqor_api_user)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
