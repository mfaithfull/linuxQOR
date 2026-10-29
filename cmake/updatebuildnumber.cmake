# Puts the generated buildnumber.h on ${target}'s include path. ${target} must
# be set before this file is included.
#
# The header is written once per configure into one directory shared by the
# whole build tree, so every module reports the same number. It used to be an
# always-dirty ${target}_buildnumber custom target per module, each with its own
# counter file: every build re-stamped every header (which re-compiled all the
# ThisModule() translation units that feed qor_pp_module_ver_string in Release),
# the per-target counters drifted so modules reported different numbers within
# one build tree, concurrent builds of the same target raced on the header, and
# one extra target per module cluttered the generated solution. The counter now
# lives in the CMake cache; see the BUILD_NUMBER block in the top level
# CMakeLists.txt.

if(NOT DEFINED target)
    message(FATAL_ERROR "updatebuildnumber.cmake expects 'target' to be set before it is included")
endif()

set(BUILDNUMBER_DIR "${CMAKE_BINARY_DIR}/generated")

get_property(_buildnumber_written GLOBAL PROPERTY QOR_BUILDNUMBER_WRITTEN)
if(NOT _buildnumber_written)
    file(MAKE_DIRECTORY "${BUILDNUMBER_DIR}")
    set(BUILDNUMBER_HEADER "${BUILDNUMBER_DIR}/buildnumber.h")
    include("${CMAKE_SOURCE_DIR}/cmake/buildnumber.cmake")
    set_property(GLOBAL PROPERTY QOR_BUILDNUMBER_WRITTEN TRUE)
endif()

# Include generated header
target_include_directories(${target} PRIVATE "${BUILDNUMBER_DIR}")
