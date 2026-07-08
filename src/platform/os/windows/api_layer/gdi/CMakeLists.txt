project(QOR VERSION 0.5.0 DESCRIPTION "QOR Win32 GDI Module" LANGUAGES CXX)
set(target winqor_api_gdi)

add_library(${target} SHARED winqor_api_gdi.cpp 
bitmap.cpp
brush.cpp
clipping.cpp
colour.cpp
coordtransform.cpp
devicecontext.cpp
drawing.cpp
text.cpp
pen.cpp
painting.cpp
path.cpp
printing.cpp
region.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINAPIGDI PUBLIC ${global_defines}
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_error qor_issue winqor_api_user)
