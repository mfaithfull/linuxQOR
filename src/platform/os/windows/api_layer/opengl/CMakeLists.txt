set(target winqor_api_opengl)

add_library(${target} SHARED winqor_api_opengl.cpp 
opengl.cpp
gl.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINAPIOPENGL PUBLIC ${global_defines}
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_error qor_issue winqor_api_user winqor_api_kernel opengl32 glu32)
