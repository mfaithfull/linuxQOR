set(target qor_stdofstreamsink)

add_library(${target} SHARED qor_stdofstreamsink.cpp stdofstreamsink.cpp stdofstreamwriter.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_STDOFSTREAMSINK
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_objectcontext qor_signals qor_pipeline 
qor_filesystem qor_stdfstreamconnector)
