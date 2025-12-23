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
mouse.cpp
view/controllers/parts/part.cpp
view/controllers/parts/basetext.cpp
view/controllers/parts/devicechange.cpp
view/controllers/parts/menu.cpp
view/controllers/parts/owner.cpp
view/controllers/parts/registration.cpp
view/controllers/parts/style.cpp
view/controllers/parts/timer.cpp
view/controllers/parts/user.cpp
view/controllers/base.cpp
view/controllers/toplevel.cpp
view/controllers/interactive.cpp
view/controllers/rendered.cpp
view/controllers/interactive/focus.cpp
view/controllers/interactive/clipboard.cpp
view/controllers/interactive/draganddrop.cpp
view/controllers/interactive/help.cpp
view/controllers/interactive/ime.cpp
view/controllers/interactive/keyboardinput.cpp
view/controllers/interactive/ncpointer.cpp
view/controllers/interactive/parent.cpp
view/controllers/interactive/pointer.cpp
view/controllers/interactive/scroll.cpp
view/controllers/interactive/winctrlparent.cpp
view/controllers/rendered/cursor.cpp
view/controllers/rendered/iconized.cpp
view/controllers/rendered/nc.cpp
view/controllers/rendered/ncrender.cpp
view/controllers/rendered/rendering.cpp
view/controllers/rendered/text.cpp
gui.cpp
messagehandler.cpp
)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINGUI)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_error qor_issue qor_thread 
qor_interception qor_signals winqor_api_kernel winqor_api_user winqor_api_gdi winqor_api_shell winqor_api_comctl32 winqor_common winqor_sharedbootstrap)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")