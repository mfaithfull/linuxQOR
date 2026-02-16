cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_http)

add_library(${target} SHARED 
qor_http.cpp 
protocol.cpp 
filter.cpp
client/requestsource/source.cpp
client/requestgenerator/requestgenerator.cpp 
client/requestgenerator/context.cpp
client/requestgenerator/state.cpp
server/requestparser/requestparser.cpp
server/requestparser/authoritytoken.cpp
server/requestparser/dec_octettoken.cpp
server/requestparser/field_contenttoken.cpp
server/requestparser/field_linetoken.cpp
server/requestparser/field_valuetoken.cpp
server/requestparser/field_vchartoken.cpp
server/requestparser/heir_parttoken.cpp
server/requestparser/hosttoken.cpp
server/requestparser/ipv4addresstoken.cpp
server/requestparser/path_abemptytoken.cpp
server/requestparser/path_rootlesstoken.cpp
server/requestparser/pchartoken.cpp
server/requestparser/pct_encodedtoken.cpp
server/requestparser/porttoken.cpp
server/requestparser/protocolnametoken.cpp
server/requestparser/protocoltoken.cpp
server/requestparser/querytoken.cpp
server/requestparser/reg_nametoken.cpp
server/requestparser/request_linetoken.cpp
server/requestparser/requesttoken.cpp
server/requestparser/schemetoken.cpp
server/requestparser/segment_nztoken.cpp
server/requestparser/segmenttoken.cpp
server/requestparser/sub_delimstoken.cpp
server/requestparser/targettoken.cpp
server/requestparser/tokennode.cpp
server/requestparser/tokentoken.cpp
server/requestparser/unreservedtoken.cpp
server/requestparser/uri_absolute_formtoken.cpp
server/requestparser/uri_absolute_pathtoken.cpp
server/requestparser/uri_asterisk_formtoken.cpp
server/requestparser/uri_authority_formtoken.cpp
server/requestparser/uri_origin_formtoken.cpp
server/requestparser/userinfotoken.cpp
client/responsesink/sink.cpp 
client/responseparser/responseparser.cpp
server/responsegenerator/responsegenerator.cpp
request/request.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_HTTP)
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection 
qor_parser)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")