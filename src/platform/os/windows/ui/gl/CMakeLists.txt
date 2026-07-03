set(target winqor_gl)

add_library(${target} SHARED winqor_gl.cpp 
cursorimpl.cpp
windowimpl.cpp
wgl.cpp
)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINDOWSGL
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_objectcontext qor_injection qor_thread qor_issue qor_error qor_gl
winqor_gui
dinput8 dwmapi xinput)
