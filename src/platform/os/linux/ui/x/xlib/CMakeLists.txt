set(target linqor_x)

add_library(${target} SHARED linqor_x.cpp xlib.cpp display.cpp pixmap.cpp window.cpp screen.cpp gc.cpp visual.cpp font.cpp
servergrab.cpp pointergrab.cpp keyboardgrab.cpp keygrab.cpp buttongrab.cpp colourmap.cpp selection.cpp cursor.cpp image.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_LINX
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_error qor_issue qor_thread 
qor_interception qor_egl linqor_thread X11)
