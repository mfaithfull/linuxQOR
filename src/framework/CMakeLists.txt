cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_COMPILE_OPTIONS_RTTI)
set(CMAKE_CXX_RTTI ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(host)
add_subdirectory(thread)
add_subdirectory(signals)
add_subdirectory(pipeline)
add_subdirectory(workflow)
add_subdirectory(role)
add_subdirectory(application)
add_subdirectory(task)
