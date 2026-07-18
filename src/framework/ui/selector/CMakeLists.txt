project(QOR VERSION 0.0.1 DESCRIPTION "QOR UI Selector Module" LANGUAGES CXX)
set(target qor_uiselector)

add_library(${target} SHARED qor_uiselector.cpp
    uiselector.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_UISELECTOR
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_current)
