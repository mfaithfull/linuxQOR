cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(target retropad)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

add_executable(${target} main.cpp fileio.cpp retropad.rc model.cpp viewmodel.cpp controller.cpp windowclasscontroller.cpp
mainwindowinstancecontroller.cpp)
set_target_properties(${target} PROPERTIES WIN32_EXECUTABLE true)
target_compile_definitions(${target} PRIVATE WINDOWS UNICODE _UNICODE QOR_PP_EXECUTABLE qor_pp_ver_major=1 )

target_link_libraries(${target} qor_module qor_host qor_compiler qor_platform qor_application 
qor_log qor_pipeline qor_asyncioservice qor_property qor_signals winqor_api_kernel winqor_api_shell winqor_common winqor_gui
user32 gdi32 comdlg32 comctl32 shell32)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")

add_custom_command(
TARGET ${target} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:${target}> $<TARGET_FILE_DIR:${target}>
COMMAND_EXPAND_LISTS
)
