set(target qor_http)

add_library(${target} SHARED qor_http.cpp request/request.cpp response/response.cpp ranges.cpp forms/forms.cpp headers.cpp
fields/fields.cpp detail/matcher.cpp detail/pathparamsmatcher.cpp detail/regexmatcher.cpp parse.cpp 
multipart.cpp response/status.cpp result.cpp error.cpp request/requestwriter.cpp response/responsereader.cpp
fields/fieldparser.cpp httpservicefilter.cpp httprequestsource.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_HTTP
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection qor_parser)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")