set(target qor_threadmemory)

add_library(${target} SHARED qor_threadmemory.cpp
fastheap/stackpage.cpp threadheap/threadheap.cpp fastheap/fastbucket.cpp fastheap/fastheap.cpp
smallobjectheap/smallobjectpage.cpp smallobjectheap/smallobjectbucket.cpp
smallobjectheap/smallobjectheap.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_THREADMEMORY
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_thread qor_sync)
