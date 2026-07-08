project(QOR VERSION 0.5.0 DESCRIPTION "QOR Log Module" LANGUAGES CXX)
set(target qor_log)

add_library(qor_log SHARED qor_log.cpp log.cpp debug.cpp important.cpp
impactful.cpp informative.cpp imperative.cpp defaultloghandler.cpp
levelwhat.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_LOG
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_current qor_interception qor_issue qor_error qor_signals)
