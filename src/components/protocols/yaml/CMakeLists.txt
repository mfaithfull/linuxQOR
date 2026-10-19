set(target qor_yaml)

add_library(${target} SHARED qor_yaml.cpp yaml.cpp
parser/states/_4/string.cpp
parser/states/_1/_false.cpp
parser/states/_1/_null.cpp
parser/states/_1/_true.cpp
parser/states/_3/mapping.cpp
parser/states/_3/pair.cpp
parser/states/_1/sequence.cpp
parser/states/_4/value.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_YAML
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_current qor_sync qor_injection qor_parser qor_workflow qor_pipeline qor_issue qor_error qor_log)