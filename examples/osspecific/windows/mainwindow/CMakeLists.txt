set(target mainwindow)

add_executable(${target} mainwindow.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

set_target_properties(${target} PROPERTIES WIN32_EXECUTABLE true)
target_compile_definitions(${target} PRIVATE WINDOWS UNICODE _UNICODE QOR_PP_EXECUTABLE
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} winqor_exebootstrap qor_module qor_host qor_compiler qor_platform winqor_api_kernel winqor_api_shell winqor_common winqor_gui
user32 gdi32 comdlg32 comctl32 shell32 uxtheme)

add_custom_command(
TARGET ${target} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
COMMAND_EXPAND_LISTS
)
