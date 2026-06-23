project(QOR VERSION 0.5.0 DESCRIPTION "QOR Interception Module" LANGUAGES CXX)
set(target qor_interception)

add_library(${target} SHARED qor_interception.cpp callcontext.cpp functioncontext.cpp
 flystrapbase.cpp flystrap.cpp callinterceptor.cpp functioncontextlock.cpp
 parameters/return.cpp)

 include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_INTERCEPTION
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_objectcontext qor_thread qor_current)
