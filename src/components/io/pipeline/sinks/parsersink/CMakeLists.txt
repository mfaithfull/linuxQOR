set(target qor_parsersink)

add_library(${target} SHARED qor_parsersink.cpp parsersink.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_PARSERSINK
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_signals qor_pipeline qor_error qor_log qor_workflow qor_parser)
