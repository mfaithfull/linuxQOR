cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_gui)

add_library(${target} SHARED winqor_gui.cpp
windowclassregistration.cpp
brush.cpp
icon.cpp
cursor.cpp
devicecontext.cpp
region.cpp
menu.cpp
window.cpp
acceleratortable.cpp
font.cpp
desktop.cpp
windowstation.cpp
rect.cpp
caret.cpp
hook.cpp
monitor.cpp
palette.cpp
colourspace.cpp
gdiobject.cpp
bitmap.cpp
pen.cpp
memdevicecontext.cpp
metadc.cpp
metafile.cpp
enhmetafile.cpp
enhmetadc.cpp
mouse.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINGUI)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_error qor_issue qor_thread 
qor_interception winqor_api_kernel winqor_api_user winqor_api_gdi winqor_common winqor_sharedbootstrap)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")