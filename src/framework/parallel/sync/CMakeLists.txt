project(QOR VERSION 0.5.0 DESCRIPTION "QOR Sync Module" LANGUAGES CXX)
set(target qor_sync)

add_library(${target} SHARED qor_sync.cpp mutex.cpp recursive_mutex.cpp 
asyncautoresetevent.cpp asyncmanualresetevent.cpp asyncmutex.cpp lightweightmanualresetevent.cpp
spinmutex.cpp spinwait.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_SYNC
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host)
