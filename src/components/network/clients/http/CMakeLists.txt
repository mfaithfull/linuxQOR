set(target qor_httpclient)

add_library(${target} qor_httpclient.cpp protohttpclient.cpp httpsocketclient.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_HTTPCLIENT
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_http qor_module qor_host qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection)

