set(target winqor_api_user)

add_library(${target} SHARED winqor_api_user.cpp
accessibility.cpp
processthread.cpp
errorhandling.cpp
windowclass.cpp
brush.cpp
cursor.cpp
help.cpp
icon.cpp
ime.cpp
message.cpp
dialog.cpp
accelerator.cpp
windowprocedure.cpp
painting.cpp
window.cpp
menu.cpp
keyboard.cpp
devicecontext.cpp
coordtransform.cpp
desktop.cpp
resource.cpp
rectangle.cpp
caret.cpp
clipboard.cpp
monitor.cpp
filledshapes.cpp
windowproperties.cpp
systemshutdown.cpp
text.cpp
timer.cpp
mouse.cpp
mdi.cpp
devicemanagement.cpp
combobox.cpp
scrollbar.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_WINAPIUSER PUBLIC ${global_defines}
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_error qor_issue winqor_api_kernel)
