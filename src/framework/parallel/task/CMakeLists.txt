set(target qor_task)

add_library(${target} SHARED qor_task.cpp
cancellation/cancellationregistration.cpp cancellation/cancellationsource.cpp
cancellation/cancellationstate.cpp cancellation/cancellationtoken.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_TASK
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_objectcontext)
