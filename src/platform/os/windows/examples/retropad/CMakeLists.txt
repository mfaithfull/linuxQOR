set(target retropad)

add_executable(${target} main.cpp fileio.cpp retropad.rc model.cpp viewmodel.cpp controller.cpp windowclasscontroller.cpp
mainwindowinstancecontroller.cpp)
set_target_properties(${target} PROPERTIES WIN32_EXECUTABLE true)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE WINDOWS UNICODE _UNICODE QOR_PP_EXECUTABLE
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform qor_application 
qor_log qor_pipeline qor_asyncioservice qor_property qor_signals winqor_api_kernel winqor_api_shell winqor_common winqor_gui
user32 gdi32 comdlg32 comctl32 shell32)

add_custom_command(
TARGET ${target} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
COMMAND_EXPAND_LISTS
)
