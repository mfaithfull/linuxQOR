set(target qor_parser)

add_library(${target} SHARED qor_parser.cpp state.cpp parser.cpp context.cpp "states/rfc5234.cpp" "states/oneormore.cpp" "states/anyoneofset.cpp" "states/anyoneof.cpp" "states/specific.cpp" "states/oneofarange.cpp" "states/optional.cpp" "states/sequence.cpp" "states/zeroormore.cpp")

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_PARSER
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection 
qor_workflow qor_pipeline)
