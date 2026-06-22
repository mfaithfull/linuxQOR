set(target qor_application)

add_library(${target} SHARED qor_application.cpp application.cpp application_builder.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_APPLICATION
qor_pp_ver_major=${PROJECT_VERSION_MAJOR} 
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_thread qor_datastructures qor_objectcontext qor_sync qor_platform qor_role)
