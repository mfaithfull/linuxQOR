set(target qor_json)

add_library(${target} SHARED qor_json.cpp nodes/string.cpp nodes/value.cpp nodes/object.cpp nodes/member.cpp nodes/array.cpp nodes/number.cpp
escape.cpp character.cpp string.cpp number.cpp integer.cpp fraction.cpp exp.cpp digit1_9.cpp 
member.cpp object.cpp array.cpp value.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_JSON
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection 
qor_workflow qor_pipeline qor_property qor_parser)
