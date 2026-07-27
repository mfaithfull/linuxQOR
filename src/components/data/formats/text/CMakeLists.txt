set(target qor_textdata)

add_library(${target} SHARED qor_textdata.cpp 
parser/states/bom.cpp
parser/states/text.cpp
parser/states/lebom.cpp
parser/states/utf8/utf8bom.cpp
parser/states/utf8/utf8char.cpp
parser/states/utf8/multibyteutf8char.cpp
parser/states/utf16/utf16bebom.cpp
parser/states/utf16/utf16bechar.cpp
parser/states/utf16/utf16lechar.cpp
parser/states/utf16/utf16lowbyte.cpp
parser/states/utf16/utf16highbyte.cpp
parser/states/utf32/utf32leindicator.cpp
parser/states/utf32/utf32bebom.cpp
parser/states/utf32/utf32bechar.cpp
parser/states/utf32/utf32lechar.cpp
parser/states/utf8/followbyte.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_TEXTDATA
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_current qor_sync qor_injection 
qor_workflow qor_pipeline qor_parser qor_issue qor_error qor_log)
