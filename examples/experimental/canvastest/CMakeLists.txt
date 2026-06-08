project(CanvasTest VERSION 0.0.1 LANGUAGES CXX)
set(target canvastest)
add_executable(${target} main.cpp)
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
target_compile_definitions(${target} PRIVATE QOR_PP_EXECUTABLE qor_pp_ver_project=${PROJECT_VERSION})

if(CMAKE_SYSTEM_NAME MATCHES Linux)
target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform qor_application qor_egl qor_opengles qor_renderer qor_canvas 
qor_widgets qor_layout linqor_egl linqor_opengles)
endif()

if(CMAKE_SYSTEM_NAME MATCHES Windows)
target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform qor_application qor_log qor_pipeline qor_asyncioservice qor_property
qor_egl qor_role qor_thread qor_widgets qor_layout winqor_thread winqor_egl qor_opengles qor_canvas qor_renderer winqor_opengles)
endif()

if (WIN32)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
    COMMAND_EXPAND_LISTS
  )
endif ()
