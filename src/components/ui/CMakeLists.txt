set(target qor_ui)

add_library(${target} SHARED qor_ui.cpp visualworkflow.cpp 
frameworkproxies/applicationproxy.cpp
frameworkproxies/roleproxy.cpp
applicationmodel.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")

target_compile_definitions(${target} PRIVATE QOR_UI
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_objectcontext qor_role qor_application qor_workflow qor_signals qor_error
qor_property)

#add_subdirectory(cairo)
add_subdirectory(canvas)
add_subdirectory(console)
add_subdirectory(egl)
add_subdirectory(layout)
add_subdirectory(opengles)
add_subdirectory(renderer)
add_subdirectory(terminal)
add_subdirectory(widgets)