set(target winqor_egl)

add_library(${target} SHARED winqor_egl.cpp egl.cpp display.cpp context.cpp window.cpp)

target_include_directories(${target} PRIVATE "F:/Develop/Native_SDK/include/")

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINEGL
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_objectcontext qor_error qor_issue qor_thread 
qor_interception qor_egl winqor_api_kernel winqor_api_user winqor_common winqor_sharedbootstrap winqor_gui ${EGL_LIBRARY})
