set(target linqor_opengles)

add_library(${target} SHARED linqor_opengles.cpp opengles.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_LINOPENGLES
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_objectcontext qor_error qor_issue qor_thread 
qor_interception qor_egl qor_opengles linqor_egl EGL GLESv2 X11)
