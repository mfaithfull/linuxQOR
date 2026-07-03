set(target linqor_waylandclient)

add_library(${target} SHARED linqor_waylandclient.cpp client.cpp
display.cpp proxy.cpp registry.cpp queue.cpp compositor.cpp surface.cpp
callback.cpp region.cpp buffer.cpp touch.cpp subsurface.cpp
subcompositor.cpp shmpool.cpp shm.cpp shellsurface.cpp shell.cpp seat.cpp
pointer.cpp keyboard.cpp datadevicemanager.cpp datasource.cpp output.cpp
datadevice.cpp dataoffer.cpp
listeners/bufferlistener.cpp 
listeners/callbacklistener.cpp
listeners/datadevicelistener.cpp
listeners/dataofferlistener.cpp 
listeners/datasourcelistener.cpp
listeners/keyboardlistener.cpp
listeners/pointerlistener.cpp 
listeners/registrylistener.cpp
listeners/seatlistener.cpp
listeners/shellsurfacelistener.cpp
listeners/shmlistener.cpp
listeners/surfacelistener.cpp
listeners/touchlistener.cpp
listeners/outputlistener.cpp
session.cpp
controllers/keyboardcontroller.cpp
controllers/pointercontroller.cpp
controllers/touchcontroller.cpp
controllers/outputcontroller.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_LINWAYLAND
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_objectcontext qor_error qor_issue qor_thread 
qor_interception qor_log qor_signals linqor_thread wayland-client)


