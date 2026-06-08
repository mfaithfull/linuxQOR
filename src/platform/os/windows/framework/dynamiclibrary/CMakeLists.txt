set(target winqor_dynamiclibrary)

add_library(${target} SHARED winqor_dynamiclibrary.cpp 
dynamiclibrary.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINDOWSDLL
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_injection qor_thread qor_issue qor_error
qor_dynamiclibrary winqor_api_kernel winqor_common)
