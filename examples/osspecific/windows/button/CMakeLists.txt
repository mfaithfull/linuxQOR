project(Button VERSION 1.0.0 DESCRIPTION "QOR Windows Scratch App" LANGUAGES CXX)
set(target button)

add_executable(${target} button.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
set_target_properties(${target} PROPERTIES WIN32_EXECUTABLE true)
target_compile_definitions(${target} PRIVATE WINDOWS UNICODE _UNICODE QOR_PP_EXECUTABLE 
qor_pp_ver_project=${PROJECT_VERSION})

target_link_libraries(${target} winqor_exebootstrap qor_module qor_host qor_compiler qor_platform winqor_api_kernel winqor_api_shell winqor_common winqor_gui
user32 gdi32 comdlg32 comctl32 shell32)

add_custom_command(
TARGET ${target} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
COMMAND_EXPAND_LISTS
)
