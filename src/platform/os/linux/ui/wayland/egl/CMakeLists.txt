set(target linqor_waylandegl)

add_library(${target} SHARED linqor_waylandegl.cpp eglwindow.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_LINWLEGL
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_error qor_issue qor_thread 
qor_interception qor_log linqor_thread linqor_waylandclient linqor_xdgshell wayland-egl wayland-client)
