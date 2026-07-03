set(target winqor_api_comctl32)

add_library(${target} SHARED winqor_api_comctl32.cpp comctl32.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINAPICOMCTL PUBLIC ${global_defines}
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_error qor_issue winqor_api_user winqor_api_gdi winqor_api_kernel)
