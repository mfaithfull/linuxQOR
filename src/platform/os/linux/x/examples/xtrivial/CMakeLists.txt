set(target xtrivial)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(${target} main.cpp)

target_include_directories(${target} PRIVATE
"/usr/include/X11")

target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})


target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform qor_application X11 Xt Xaw)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")
