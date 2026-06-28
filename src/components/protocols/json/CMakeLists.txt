set(target qor_json)

add_library(${target} SHARED qor_json.cpp 
parser/_1/array.cpp
parser/_2/escape.cpp
parser/_2/character.cpp
parser/_4/string.cpp
parser/_4/value.cpp
parser/_3/fraction.cpp
parser/_3/integer.cpp
parser/_3/member.cpp
parser/_3/number.cpp
parser/_3/object.cpp
parser/_2/exp.cpp
parser/_2/digit1_9.cpp
parser/_2/escapesequence.cpp
parser/_2/unicodesequence.cpp
parser/_1/_false.cpp
parser/_1/_null.cpp
parser/_1/_true.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_JSON
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection 
qor_workflow qor_pipeline qor_property qor_parser)
