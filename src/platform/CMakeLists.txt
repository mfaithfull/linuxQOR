set(target qor_platform)

add_library(${target} SHARED qor_platform.cpp platform.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_PLATFORM
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_sync qor_thread)

add_subdirectory(compiler)
add_subdirectory(architecture)
add_subdirectory(os)
add_subdirectory(filesystem)
add_subdirectory(network)
