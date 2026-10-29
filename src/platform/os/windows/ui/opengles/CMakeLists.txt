set(target winqor_opengles)

add_library(${target} SHARED winqor_opengles.cpp opengles.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINOPENGLES
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH}
qor_opengles_egl_include_dir=${ANGLE_INCLUDE_DIR})

target_include_directories(${target} PRIVATE "${ANGLE_INCLUDE_DIR}")

target_link_libraries(${target} qor_module qor_host qor_error qor_issue qor_current
qor_interception qor_egl qor_opengles winqor_api_kernel winqor_api_user winqor_common winqor_sharedbootstrap winqor_gui OpenGLES3)
