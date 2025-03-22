cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_executable(QORTest test_main.cpp test_test.cpp test_compiler.cpp)
target_link_libraries(QORTest QORCompiler)