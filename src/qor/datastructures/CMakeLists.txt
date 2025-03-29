cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_datastructures SHARED datastructures.cpp guid.cpp)
target_compile_definitions(qor_datastructures PUBLIC -DQOR_DATASTRUCTURES)
target_link_libraries(qor_datastructures qor_module qor_host)
