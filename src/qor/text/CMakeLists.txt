set(target qor_text)

add_library(${target} SHARED 
qor_text.cpp
codepage/codepages/c.cpp
codepage/codepages/ascii.cpp
codepage/codepages/utf16.cpp
codepage/codepages/utf32.cpp
codepage/codepages/isolatin1.cpp
codepage/codepages/utf8.cpp
codepage/codepages/isolatin2.cpp
codepage/codepages/iso8859-3.cpp
codepage/codepages/iso8859-4.cpp
codepage/codepageregistry.cpp
codepage/codepages.cpp
buffers/constbuffer.cpp
buffers/mutablebuffer.cpp
buffers/itexterrordi.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_TEXT
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_objectcontext)
