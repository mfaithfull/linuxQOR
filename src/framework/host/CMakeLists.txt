set(target qor_host)

add_library(${target} SHARED host.cpp host_module.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_PP_HOST 
qor_pp_ver_major=${PROJECT_VERSION_MAJOR} 
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries(${target} qor_module winqor_sharedbootstrap)
endif()
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(${target} qor_module)
endif()
