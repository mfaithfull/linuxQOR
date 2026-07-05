set(target winqor_thread)

add_library(${target} SHARED winqor_thread.cpp currentthread.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINDOWSTHREAD
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH}
qor_pp_implements_icurrentthread)

target_link_libraries(${target} qor_module qor_host qor_sync qor_error)
