cmake_minimum_required(VERSION 3.20.0)
project(QOR VERSION 0.5.0 DESCRIPTION "Querysoft Open Runtime" LANGUAGES C CXX)
set(CMAKE_SUPPRESS_REGENERATION true)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_RTTI ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

if(WIN32)
    # Windows has no system OpenGL ES; use ANGLE (Chromium's GLES implementation).
    # Its libGLESv2.dll implements the OpenGL ES 3.0 API surface.
    find_path(ANGLE_INCLUDE_DIR
              NAMES GLES3/gl3.h EGL/egl.h
              PATHS ${CMAKE_SOURCE_DIR}/3rdparty/angle/angle-x64/include
                    "C:/Program Files/angle/include")

    find_library(EGL_LIBRARY
                 NAMES libEGL egl
                 PATHS ${CMAKE_SOURCE_DIR}/3rdparty/angle/angle-x64/lib
                       "C:/Program Files/angle/lib")

    find_library(OPENGLES3_LIBRARY
                 NAMES libGLESv2 GLESv2
                 PATHS ${CMAKE_SOURCE_DIR}/3rdparty/angle/angle-x64/lib
                       "C:/Program Files/angle/lib")

    if(NOT (ANGLE_INCLUDE_DIR AND EGL_LIBRARY AND OPENGLES3_LIBRARY))
        message(FATAL_ERROR "OpenGL ES 3.0 (ANGLE) not found. Install ANGLE or point CMake at it, e.g. -DANGLE_ROOT=C:/angle")
    endif()

    # Wrap the results in an INTERFACE target so any subdirectory can link it:
    add_library(OpenGLES3 INTERFACE)
    target_include_directories(OpenGLES3 INTERFACE ${ANGLE_INCLUDE_DIR})
    target_link_libraries(OpenGLES3 INTERFACE ${EGL_LIBRARY} ${OPENGLES3_LIBRARY})
endif()

if(MSVC)
    add_link_options(
        $<$<CONFIG:RelWithDebInfo>:/INCREMENTAL:NO>
    )
endif()

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(examples)
