set(target qor_echo)

add_library(${target} SHARED qor_echo.cpp echoprotocol.cpp
echoresponsefilter.cpp echoresponseparser.cpp
echoservicefilter.cpp echorequestparser.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_ECHO
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_current qor_sync qor_injection 
qor_parser)
