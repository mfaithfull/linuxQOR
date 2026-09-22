set(target qor_jip)

add_library(${target} SHARED qor_jip.cpp jipprotocol.cpp
jipresponsefilter.cpp jipresponseparser.cpp
jipservicefilter.cpp jiprequestparser.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_JIP
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_current qor_sync qor_injection 
qor_parser)
