set(target qor_asyncioservice)

add_library(${target} SHARED qor_asyncioservice.cpp service.cpp initiator.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_ASYNCIOSERVICE
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_injection qor_current qor_thread qor_interception qor_sync qor_issue qor_error qor_log)
