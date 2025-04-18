cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_task SHARED qor_task.cpp
cancellation/cancellationregistration.cpp cancellation/cancellationsource.cpp
cancellation/cancellationstate.cpp cancellation/cancellationtoken.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines -fno-threadsafe-statics -fvisibility=hidden")
target_compile_definitions(qor_task PUBLIC -DQOR_TASK)
target_link_libraries(qor_task qor_module qor_host qor_datastructures qor_objectcontext)
