set(target qor_socketconnector)

add_library(${target} SHARED qor_socketconnector.cpp socketconnector.cpp socketsink.cpp
socketwriter.cpp socketreader.cpp socketsource.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_SOCKETCONNECTOR
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_thread 
qor_signals qor_pipeline qor_platform qor_sockets qor_asyncioservice qor_error)
