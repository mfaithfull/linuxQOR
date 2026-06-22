set(target winqor_sockets)

add_library(${target} SHARED winqor_sockets.cpp sockets.cpp socket.cpp addresshelpers.h)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE WINQOR_SOCKETS
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_sockets qor_module qor_host qor_datastructures qor_objectcontext qor_sync qor_thread qor_platform
winqor_asyncioservice winqor_api_winsock2)
