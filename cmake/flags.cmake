if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines -fvisibility=hidden -fconcepts-diagnostics-depth=3 -Wno-attributes -Wall")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /GR /utf-8")
endif()

set(global_defines -Dqor_pp_unicode)