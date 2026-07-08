project(QOR VERSION 0.5.0 DESCRIPTION "QOR Error Module" LANGUAGES CXX)
set(target qor_error)

add_library(qor_error SHARED qor_error.cpp errorbase.cpp fatal.cpp serious.cpp
continuable.cpp warning.cpp note.cpp clibresult.cpp commonerrormessages.cpp 
defaulterrorhandler.cpp reportingimpl.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_ERROR
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_issue qor_text qor_current qor_interception qor_reporting)
