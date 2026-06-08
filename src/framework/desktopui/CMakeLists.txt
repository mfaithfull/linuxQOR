set(target qor_desktopui)

add_library(${target} SHARED qor_desktopui.cpp desktopui.cpp
    layout/box.cpp
    layout/node.cpp
    layout/nodes/vbox.cpp
    layout/nodes/hbox.cpp
    layout/nodes/size.cpp
    layout/nodes/reflect.cpp
    layout/nodes/nodedecorator.cpp
    layout/nodes/detail/boxhelper.cpp
    layout/nodes/dbox.cpp
    layout/functional/compositedecorator.cpp
    layout/functional/flex.cpp
    layout/functional/flexboxconfig.cpp
    layout/functional/flexbox.cpp
    layout/functional/flexboxhelper.cpp
    layout/functional/layout.cpp
    widgets/pushbutton.cpp
    windows/window.cpp
    windows/mainwindow.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_DESKTOPUI
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_datastructures qor_objectcontext qor_sync qor_thread
qor_role)
