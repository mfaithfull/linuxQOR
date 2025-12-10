cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(target helloworld)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(${target} main.cpp)

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

target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE qor_pp_ver_major=1 )

target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform qor_application 
gtk-3 gdk-3 pangocairo-1.0 pango-1.0 harfbuzz atk-1.0 cairo-gobject cairo gdk_pixbuf-2.0 gio-2.0 gobject-2.0 glib-2.0)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
