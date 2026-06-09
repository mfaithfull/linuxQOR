set(target linqor_xegl)

add_library(${target} SHARED linqor_xegl.cpp eglsession.cpp eglwindow.cpp egldisplay.cpp)

target_include_directories(${target} PRIVATE)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")

target_compile_definitions(${target} PRIVATE QOR_LINWLEGL
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_error qor_issue qor_thread 
qor_interception qor_log linqor_thread linqor_x qor_egl
X11)


