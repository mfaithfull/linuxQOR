set(target qor_jsondata)

add_library(${target} SHARED qor_json.cpp 
parser/states/_1/array.cpp
parser/states/_2/escape.cpp
parser/states/_2/character.cpp
parser/states/_4/string.cpp
parser/states/_4/value.cpp
parser/states/_3/fraction.cpp
parser/states/_3/integer.cpp
parser/states/_3/member.cpp
parser/states/_3/number.cpp
parser/states/_3/object.cpp
parser/states/_2/exp.cpp
parser/states/_2/digit1_9.cpp
parser/states/_2/escapesequence.cpp
parser/states/_2/unicodesequence.cpp
parser/states/_1/_false.cpp
parser/states/_1/_null.cpp
parser/states/_1/_true.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_JSON
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_current qor_sync qor_injection 
qor_workflow qor_pipeline qor_parser qor_issue qor_error qor_log)
