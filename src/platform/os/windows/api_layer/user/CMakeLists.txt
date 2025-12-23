cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_api_user)

add_library(${target} SHARED winqor_api_user.cpp
processthread.cpp
errorhandling.cpp
windowclass.cpp
brush.cpp
cursor.cpp
icon.cpp
message.cpp
dialog.cpp
accelerator.cpp
windowprocedure.cpp
painting.cpp
window.cpp
menu.cpp
keyboard.cpp
devicecontext.cpp
coordtransform.cpp
desktop.cpp
resource.cpp
rectangle.cpp
caret.cpp
clipboard.cpp
monitor.cpp
filledshapes.cpp
windowproperties.cpp
systemshutdown.cpp
text.cpp
mouse.cpp
mdi.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINAPIUSER PUBLIC ${global_defines})
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_error qor_issue winqor_api_kernel)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")