set(target qor_logaggregator)

add_library(${target} SHARED qor_logaggregator.cpp
logaggregator.cpp logreceiver.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_LOGAGGREGATOR
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_thread qor_sync qor_signals qor_error qor_log qor_interception qor_loghandler qor_filesystem qor_role)
