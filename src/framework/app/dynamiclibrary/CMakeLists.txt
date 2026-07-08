project(QOR VERSION 0.0.5 DESCRIPTION "QOR Dynamic Library Module" LANGUAGES CXX)
set(target qor_dynamiclibrary)

add_library(${target} SHARED qor_dynamiclibrary.cpp
dynamiclibrary.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_DYNAMICLIBARY
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(${target} qor_module qor_host qor_thread winqor_sharedbootstrap winqor_thread)
endif()
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(${target} qor_module qor_host qor_thread linqor_thread)
endif()
