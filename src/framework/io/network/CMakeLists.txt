set(target qor_sockets)

add_library(${target} SHARED qor_sockets.cpp sockets.cpp socket.cpp address.cpp )
#"ipaddress.h" "ipv4address.h" "ipv6address.h" "ipendpoint.h" "ipv4endpoint.h" "ipv6endpoint.h" "ipv4address.cpp" "ipv6address.cpp" "ipv4endpoint.cpp" "ipaddress.cpp" "ipendpoint.cpp" "ipv6endpoint.cpp"
include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_SOCKETS
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

set(portable_libraries qor_module qor_host qor_current qor_sync qor_platform qor_issue qor_interception
qor_asyncioservice)

if(CMAKE_SYSTEM_NAME MATCHES Linux)
    set(os_libraries)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
    set(os_libraries ws2_32)
endif()

target_link_libraries(${target} ${portable_libraries} ${os_libraries})
