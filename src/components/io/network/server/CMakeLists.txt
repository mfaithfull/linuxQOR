set(target qor_netserver)

add_library(${target} SHARED qor_netserver.cpp netserver.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_NETSERVER
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_datastructures 
qor_objectcontext qor_thread qor_sync qor_injection qor_log qor_loghandler qor_logaggregator
qor_error qor_sockets qor_socketconnector qor_workflow qor_role qor_application)
