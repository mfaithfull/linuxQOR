set(target winqor_exebootstrap)

add_library(${target} winqorexeboot.cpp)

include ("${CMAKE_SOURCE_DIR}/cmake/updatebuildnumber.cmake")
include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_link_libraries(${target})
