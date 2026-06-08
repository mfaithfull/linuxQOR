set(target qor_filesystem)

add_library(${target} SHARED qor_filesystem.cpp filesystem.cpp path.cpp root.cpp fileindex.cpp folder.cpp 
file/file.cpp file/filereader.cpp file/filewriter.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_FILESYSTEM
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_sync qor_thread qor_platform qor_error)
