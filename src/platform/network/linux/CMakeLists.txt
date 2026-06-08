add_library(linqor_sockets SHARED linqor_sockets.cpp sockets.cpp socket.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(linqor_sockets PRIVATE LINQOR_SOCKETS
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(linqor_sockets qor_sockets qor_module qor_host qor_datastructures qor_objectcontext qor_sync qor_thread qor_platform
linqor_asyncioservice)
