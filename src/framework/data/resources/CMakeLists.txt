set(target qor_resources)

add_library(${target} SHARED qor_resources.cpp manager.cpp resourcehub.cpp resource.cpp
types/pathresource.cpp
types/fileresource.cpp
types/json/jsonresource.cpp
types/font/fontresource.cpp
types/font/reader.cpp
types/font/reader/designator.cpp
types/font/reader/ttfinitial.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_RESOURCES
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_thread qor_sync qor_platform qor_error qor_filesystem
qor_application qor_signals qor_log qor_pipeline qor_fileconnector qor_json qor_parsersink qor_parser qor_workflow 
qor_deserializersink qor_serialize)

add_subdirectory(image)