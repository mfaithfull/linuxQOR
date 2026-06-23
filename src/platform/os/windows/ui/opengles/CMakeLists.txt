set(target winqor_opengles)

add_library(${target} SHARED winqor_opengles.cpp opengles.cpp)

target_include_directories(${target} PRIVATE "F:/Develop/Native_SDK/include/")

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINOPENGLES
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_error qor_issue qor_current
qor_interception qor_egl qor_opengles winqor_api_kernel winqor_api_user winqor_common winqor_sharedbootstrap winqor_gui 
"F:/Develop/Native_SDK/lib/Windows_x86_64/libGLESv2.lib")
