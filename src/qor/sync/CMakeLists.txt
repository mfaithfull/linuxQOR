cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_sync SHARED qor_sync.cpp mutex.cpp recursive_mutex.cpp 
asyncautoresetevent.cpp asyncmanualresetevent.cpp asyncmutex.cpp lightweightmanualresetevent.cpp
spinmutex.cpp spinwait.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fcoroutines -fno-threadsafe-statics -fvisibility=hidden")
target_compile_definitions(qor_sync PRIVATE -DQOR_SYNC)
target_link_libraries(qor_sync qor_module qor_host)
