set(target linqor_gtkgui)

add_library(${target} SHARED linqor_gtkgui.cpp widget.cpp window.cpp application.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_include_directories(${target} PRIVATE
"/usr/include/gtk-3.0"
/usr/include/at-spi2-atk/2.0 
/usr/include/at-spi-2.0 
/usr/include/dbus-1.0 
/usr/lib/x86_64-linux-gnu/dbus-1.0/include 
/usr/include/gtk-3.0 
/usr/include/gio-unix-2.0 
/usr/include/cairo 
/usr/include/pango-1.0 
/usr/include/fribidi 
/usr/include/harfbuzz 
/usr/include/atk-1.0 
/usr/include/cairo 
/usr/include/pixman-1 
/usr/include/uuid 
/usr/include/freetype2 
/usr/include/libpng16 
/usr/include/gdk-pixbuf-2.0 
/usr/include/libmount 
/usr/include/blkid 
/usr/include/glib-2.0 
/usr/lib/x86_64-linux-gnu/glib-2.0/include)

target_compile_definitions(${target} PRIVATE QOR_LINUXGTKGUI
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform 
gtk-3 gdk-3 pangocairo-1.0 pango-1.0 harfbuzz atk-1.0 cairo-gobject cairo gdk_pixbuf-2.0 gio-2.0 gobject-2.0 glib-2.0)
