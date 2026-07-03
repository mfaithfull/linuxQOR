set(target linqor_asyncioservice)

add_library(${target} SHARED linqor_asyncioservice.cpp asyncioservice.cpp 
iouringservice/iouring.cpp iouringservice/iouringeventprocessor.cpp iouringservice/iouringinitiator.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE QOR_LINUXASYNCIOSERVICE
qor_pp_ver_major=${PROJECT_VERSION_MAJOR}
qor_pp_ver_minor=${PROJECT_VERSION_MINOR}
qor_pp_ver_patch=${PROJECT_VERSION_PATCH})

target_link_libraries(${target} qor_module qor_host qor_objectcontext qor_injection qor_asyncioservice qor_error qor_log uring rt)