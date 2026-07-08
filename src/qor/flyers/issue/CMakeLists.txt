project(QOR VERSION 0.5.0 DESCRIPTION "QOR Issue Module" LANGUAGES CXX)
set(target qor_issue)

add_library(${target} SHARED qor_issue.cpp when.cpp where.cpp what.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_ISSUE
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_current)
