project(QOR VERSION 0.5.0 DESCRIPTION "QOR Win32 Theme Module" LANGUAGES CXX)
set(target winqor_api_uxtheme)

add_library(${target} SHARED winqor_api_uxtheme.cpp 
uxtheme.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINAPIUXTHEME PUBLIC ${global_defines}
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_error qor_issue winqor_api_kernel uxtheme)
