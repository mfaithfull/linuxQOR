set(target linqor_wlpresentationtime)

add_library(${target} SHARED linqor_wlpresentationtime.cpp presentation-time.c
presentation.cpp feedback.cpp
listeners/presentationlistener.cpp
listeners/feedbacklistener.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_LINWLPRESENTATIONTIME
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_error qor_issue qor_thread 
qor_interception qor_log linqor_thread linqor_waylandclient wayland-client)


