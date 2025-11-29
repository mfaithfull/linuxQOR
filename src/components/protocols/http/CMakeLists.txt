cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_http)

add_library(${target} SHARED qor_http.cpp protocol.cpp filter.cpp
request/source.cpp request/parser/requestparser.cpp
request/parser/authoritytoken.cpp
request/parser/dec_octettoken.cpp
request/parser/field_contenttoken.cpp
request/parser/field_linetoken.cpp
request/parser/field_valuetoken.cpp
request/parser/field_vchartoken.cpp
request/parser/heir_parttoken.cpp
request/parser/hosttoken.cpp
request/parser/ipv4addresstoken.cpp
request/parser/path_abemptytoken.cpp
request/parser/path_rootlesstoken.cpp
request/parser/pchartoken.cpp
request/parser/pct_encodedtoken.cpp
request/parser/porttoken.cpp
request/parser/protocolnametoken.cpp
request/parser/protocoltoken.cpp
request/parser/querytoken.cpp
request/parser/reg_nametoken.cpp
request/parser/request_linetoken.cpp
request/parser/requesttoken.cpp
request/parser/schemetoken.cpp
request/parser/segment_nztoken.cpp
request/parser/segmenttoken.cpp
request/parser/sub_delimstoken.cpp
request/parser/targettoken.cpp
request/parser/tokennode.cpp
request/parser/tokentoken.cpp
request/parser/unreservedtoken.cpp
request/parser/uri_absolute_formtoken.cpp
request/parser/uri_absolute_pathtoken.cpp
request/parser/uri_asterisk_formtoken.cpp
request/parser/uri_authority_formtoken.cpp
request/parser/uri_origin_formtoken.cpp
request/parser/userinfotoken.cpp
response/sink.cpp response/responseparser.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_HTTP)
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection 
qor_parser)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")