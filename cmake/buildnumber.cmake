# Writes ${BUILDNUMBER_HEADER} defining qor_pp_buildnumber as ${BUILD_NUMBER}.
# Both variables must be set by the caller (see updatebuildnumber.cmake).
#
# The header is only rewritten when its contents would actually change, so its
# timestamp - and therefore every translation unit that includes it - is left
# alone whenever the build number has not moved.

if(NOT DEFINED BUILD_NUMBER OR NOT DEFINED BUILDNUMBER_HEADER)
    message(FATAL_ERROR "buildnumber.cmake requires BUILD_NUMBER and BUILDNUMBER_HEADER to be set")
endif()

if(NOT BUILD_NUMBER MATCHES "^[0-9]+$")
    message(FATAL_ERROR "buildnumber.cmake requires BUILD_NUMBER to be a non-negative integer, got '${BUILD_NUMBER}'")
endif()

set(_buildnumber_contents "#define qor_pp_buildnumber ${BUILD_NUMBER}\n")

set(_buildnumber_write TRUE)
if(EXISTS "${BUILDNUMBER_HEADER}")
    file(READ "${BUILDNUMBER_HEADER}" _buildnumber_existing)
    if(_buildnumber_existing STREQUAL _buildnumber_contents)
        set(_buildnumber_write FALSE)
    endif()
endif()

if(_buildnumber_write)
    file(WRITE "${BUILDNUMBER_HEADER}" "${_buildnumber_contents}")
    message(STATUS "Build number ${BUILD_NUMBER} -> ${BUILDNUMBER_HEADER}")
else()
    message(STATUS "Build number ${BUILD_NUMBER} (unchanged)")
endif()
