cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_host SHARED host.cpp host_module.cpp)
target_compile_definitions(qor_host PUBLIC -DQOR_PP_HOST)
target_link_libraries(qor_host qor_module winqsharedbootstrap)