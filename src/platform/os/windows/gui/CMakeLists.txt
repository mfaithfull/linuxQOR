cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_gui)

add_library(${target} SHARED winqor_gui.cpp
gui.cpp
perthread.cpp
window.cpp
windows/dialog.cpp
windows/button.cpp
windows/edit.cpp
windows/static.cpp
windows/messagebox.cpp
windows/desktop.cpp
windows/listbox.cpp
windows/combobox.cpp
controllers/caret.cpp
controllers/menu.cpp
controllers/monitor.cpp
controllers/mouse.cpp
controllers/windowcontroller.cpp
controllers/buttoncontroller.cpp
controllers/editcontroller.cpp
controllers/listboxcontroller.cpp
controllers/comboboxcontroller.cpp
factories/windowclassregistration.cpp
factories/windowfactory.cpp
factories/menubuilder.cpp
factories/windowstyle.cpp
factories/classstyle.cpp
factories/dialogbuilder.cpp
gdiobjects/bitmap.cpp
gdiobjects/brush.cpp
gdiobjects/colourspace.cpp
gdiobjects/cursor.cpp
gdiobjects/devicecontext.cpp
gdiobjects/enhmetadc.cpp
gdiobjects/enhmetafile.cpp
gdiobjects/font.cpp
gdiobjects/gdiobject.cpp
gdiobjects/icon.cpp
gdiobjects/memdevicecontext.cpp
gdiobjects/metadc.cpp
gdiobjects/metafile.cpp
gdiobjects/palette.cpp
gdiobjects/pen.cpp
gdiobjects/region.cpp
view/drawing/rect.cpp
view/handlers/acceleratortable.cpp
view/handlers/base.cpp
view/handlers/hook.cpp
view/handlers/messagehandler.cpp
view/handlers/toplevel.cpp
view/handlers/interactive.cpp
view/handlers/rendered.cpp
view/handlers/parts/part.cpp
view/handlers/parts/basetext.cpp
view/handlers/parts/devicechange.cpp
view/handlers/parts/menu.cpp
view/handlers/parts/owner.cpp
view/handlers/parts/style.cpp
view/handlers/parts/timer.cpp
view/handlers/parts/user.cpp
view/handlers/interactive/focus.cpp
view/handlers/interactive/clipboard.cpp
view/handlers/interactive/draganddrop.cpp
view/handlers/interactive/help.cpp
view/handlers/interactive/ime.cpp
view/handlers/interactive/keyboardinput.cpp
view/handlers/interactive/ncpointer.cpp
view/handlers/interactive/parent.cpp
view/handlers/interactive/pointer.cpp
view/handlers/interactive/scroll.cpp
view/handlers/interactive/winctrlparent.cpp
view/handlers/rendered/cursor.cpp
view/handlers/rendered/iconized.cpp
view/handlers/rendered/nc.cpp
view/handlers/rendered/ncrender.cpp
view/handlers/rendered/dwm.cpp
view/handlers/rendered/rendering.cpp
view/handlers/rendered/font.cpp
view/handlers/parts/lifetime.cpp
view/handlers/dialoghandler.cpp
controllers/popupmenu.cpp
painting/bufferedpainting.cpp
painting/themedata.cpp
painting/paintsession.cpp
painting/windowpainter.cpp
view/layout/layout.cpp
view/layout/windowlayoutitem.cpp
view/layout/vbox.cpp
)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINGUI)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_error qor_issue qor_thread 
qor_interception qor_signals 
winqor_api_kernel winqor_api_user winqor_api_gdi winqor_api_shell winqor_api_shcore winqor_api_comctl32 winqor_api_dwm winqor_api_uxtheme
winqor_common winqor_sharedbootstrap)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")