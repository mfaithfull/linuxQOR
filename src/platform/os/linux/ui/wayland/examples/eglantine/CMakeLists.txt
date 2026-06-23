set(target eglantine)

add_executable(${target} main.cpp eltkeycontroller.cpp eltpointercontroller.cpp eltsession.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE 
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform qor_application linqor_waylandclient linqor_xdgshell linqor_waylandegl
linqor_egl linqor_opengles
wayland-egl wayland-client EGL GL)
