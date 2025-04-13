cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(winq_filesystem winq_filesystem.cpp)
target_link_libraries(winq_filesystem)