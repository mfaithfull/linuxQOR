cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_signals SHARED qor_signals.cpp slotbase.cpp signalbase.cpp pendingslot.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-threadsafe-statics -fvisibility=hidden")
target_compile_definitions(qor_signals PUBLIC -DQOR_SIGNALS)
target_link_libraries(qor_signals qor_module qor_thread qor_datastructures qor_objectcontext)