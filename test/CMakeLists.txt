cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_COMPILE_OPTIONS_RTTI)
set(CMAKE_CXX_RTTI ON)
add_subdirectory(mock)

add_executable(QORTest test_main.cpp test_test.cpp test_compiler.cpp 
mock/test_mock.cpp mock/test_isvirtual.cpp mock/test_args.cpp
mock/test_zombie.cpp)
target_link_libraries(QORTest QORCompiler)
