cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_error SHARED qor_error.cpp severityissue.cpp fatal.cpp serious.cpp
continuable.cpp warning.cpp note.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-threadsafe-statics -fvisibility=hidden")
target_compile_definitions(qor_issue PUBLIC -DQOR_ERROR)
target_link_libraries(qor_error qor_issue qor_module qor_host qor_issue qor_thread qor_interception)
