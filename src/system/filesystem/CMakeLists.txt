cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_filesystem SHARED qor_filesystem.cpp filesystem.cpp path.cpp root.cpp fileindex.cpp folder.cpp file.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-threadsafe-statics -fvisibility=hidden")
target_compile_definitions(qor_filesystem PUBLIC -DQOR_FILESYSTEM)
target_link_libraries(qor_filesystem qor_module qor_host qor_datastructures qor_objectcontext)
