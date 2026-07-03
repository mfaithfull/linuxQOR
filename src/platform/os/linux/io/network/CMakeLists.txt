set(target linqor_sockets)

add_library(${target} SHARED linqor_sockets.cpp sockets.cpp socket.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE LINQOR_SOCKETS
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_sockets qor_module qor_host qor_objectcontext qor_sync qor_thread qor_platform
linqor_asyncioservice)
