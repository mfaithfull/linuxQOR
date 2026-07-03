set(target winqor_api_winsock2)

add_library(${target} SHARED winqor_api_winsock2.cpp ws2.cpp errorcheck.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINAPI_WINSOCK2 PUBLIC ${global_defines}
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_objectcontext qor_error qor_issue winqor_api_kernel wsock32 Ws2_32)
