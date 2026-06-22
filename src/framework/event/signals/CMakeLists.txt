set(target qor_signals)

add_library(qor_signals SHARED qor_signals.cpp slotbase.cpp signalbase.cpp pendingslot.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_SIGNALS
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_thread qor_datastructures qor_objectcontext)
