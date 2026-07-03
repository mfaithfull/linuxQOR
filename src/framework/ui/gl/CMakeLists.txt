set(target qor_gl)

add_library(${target} SHARED qor_gl.cpp
    platform.cpp 
    platformimpl.cpp 
    monitor/monitor.cpp
    monitor/monitorpimpl.cpp
    input/input.cpp
    input/joystickpimpl.cpp
    input/inputpimpl.cpp
    context/ctxconfig.cpp
    context/context.cpp
    context/contextpimpl.cpp
    cursor/cursorpimpl.cpp
    session.cpp
    window/window.cpp
    window/windowpimpl.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_GL
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_objectcontext qor_thread qor_error)
