cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target winqor_gui)

add_library(${target} SHARED winqor_gui.cpp
gui.cpp
perthread.cpp
window.cpp
wglcontext.cpp
windows/a_h/animation.cpp
windows/a_h/button.cpp
windows/a_h/combobox.cpp
windows/a_h/comboboxex.cpp
windows/a_h/datetimepicker.cpp
windows/a_h/desktop.cpp
windows/a_h/dialog.cpp
windows/a_h/edit.cpp
windows/a_h/flatscrollbar.cpp
windows/s_s/static.cpp
windows/i_m/messagebox.cpp
windows/i_m/listbox.cpp
windows/s_s/scrollbar.cpp
windows/i_m/mdiclient.cpp
controllers/a_h/animationcontroller.cpp
controllers/a_h/caret.cpp
controllers/i_m/menu.cpp
controllers/i_m/monitor.cpp
controllers/i_m/mouse.cpp
controllers/windowcontroller.cpp
controllers/s_s/staticcontroller.cpp
controllers/a_h/buttoncontroller.cpp
controllers/a_h/editcontroller.cpp
controllers/i_m/listboxcontroller.cpp
controllers/a_h/comboboxcontroller.cpp
controllers/s_s/scrollbarcontroller.cpp
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
controllers/n_r/popupmenu.cpp
painting/bufferedpainting.cpp
painting/themedata.cpp
painting/paintsession.cpp
painting/windowpainter.cpp
view/layout/layout.cpp
view/layout/windowlayoutitem.cpp
view/layout/vbox.cpp
openglsession.cpp
factories/windowclass.cpp
)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINGUI)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_error qor_issue qor_thread 
qor_interception qor_signals 
winqor_api_kernel winqor_api_user winqor_api_gdi winqor_api_shell 
winqor_api_shcore winqor_api_comctl32 winqor_api_dwm winqor_api_uxtheme
winqor_api_opengl
winqor_dynamiclibrary winqor_common winqor_sharedbootstrap)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")