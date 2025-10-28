cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_issue SHARED qor_issue.cpp when.cpp where.cpp what.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-threadsafe-statics -fvisibility=hidden")
target_compile_definitions(qor_issue PRIVATE -DQOR_ISSUE)
target_link_libraries(qor_issue qor_module qor_host qor_thread)
