set(target testxv)

add_executable(${target} main.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")

target_include_directories(${target} PRIVATE
"/usr/include/X11")

target_compile_definitions(${target} PRIVATE 
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform qor_application linqor_xvideo X11 Xext Xv)
