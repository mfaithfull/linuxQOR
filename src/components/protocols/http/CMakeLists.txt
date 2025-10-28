cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_http SHARED qor_http.cpp request.cpp response.cpp ranges.cpp forms.cpp headers.cpp
fields/fields.cpp detail/matcher.cpp detail/pathparamsmatcher.cpp detail/regexmatcher.cpp parse.cpp 
multipart.cpp status.cpp result.cpp error.cpp requestwriter.cpp responsereader.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines -fno-threadsafe-statics -fvisibility=hidden")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

target_compile_definitions(qor_http PRIVATE -DQOR_HTTP)
target_link_libraries(qor_http qor_module qor_host qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection)