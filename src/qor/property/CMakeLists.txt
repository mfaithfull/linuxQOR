project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_library(qor_property SHARED qor_property.cpp propertyvalue.cpp "propertyvaluebase.h" "propertybase.h" "propertygenerator.h")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines -fno-threadsafe-statics -fvisibility=hidden")
endif()
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

target_compile_definitions(qor_property PUBLIC -DQOR_PROPERTY)
target_link_libraries(qor_property qor_module qor_datastructures qor_objectcontext)