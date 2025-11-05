cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_http)

add_library(${target} SHARED qor_http.cpp request.cpp response.cpp ranges.cpp forms.cpp headers.cpp
fields/fields.cpp detail/matcher.cpp detail/pathparamsmatcher.cpp detail/regexmatcher.cpp parse.cpp 
multipart.cpp status.cpp result.cpp error.cpp requestwriter.cpp responsereader.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE -DQOR_HTTP)
target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")