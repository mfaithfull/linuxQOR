set(target qor_pipeline)

add_library(${target} SHARED qor_pipeline.cpp buffer.cpp element.cpp source.cpp iosource.cpp sink.cpp
filter.cpp pipeline.cpp plug.cpp iosink.cpp  copyfilter.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_PIPELINE
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_thread qor_signals qor_sync qor_interception qor_error qor_log)
