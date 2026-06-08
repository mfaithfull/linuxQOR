set(target qor_console)

add_library(${target} SHARED qor_console.cpp console.cpp consolerenderer.cpp consoleview.cpp 
consoleviewcontroller.cpp consoleviewmodel.cpp tokenbuffer.cpp consolecompositor.cpp consolelabelwidget.cpp
consoleui.cpp consolewidget.cpp consolecolumnwidget.cpp consolemenuwidget.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_CONSOLE
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_thread qor_sync qor_injection qor_role)
