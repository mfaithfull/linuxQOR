set(target qor_layout)

add_library(${target} SHARED qor_layout.cpp box.cpp node.cpp boxhelper.cpp vbox.cpp hbox.cpp size.cpp nodedecorator.cpp
flexboxhelper.cpp flexboxconfig.cpp flexbox.cpp dbox.cpp gridbox.cpp reflect.cpp operators.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_LAYOUT
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_objectcontext qor_role qor_application qor_error)
