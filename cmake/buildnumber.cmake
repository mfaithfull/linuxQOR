# build_number.cmake
if(EXISTS ${CACHE_FILE})
    file(READ ${CACHE_FILE} BUILD_NUMBER)
    math(EXPR BUILD_NUMBER "${BUILD_NUMBER}+1")    
else()
    set(BUILD_NUMBER "1")
endif()

file(WRITE ${CACHE_FILE} "${BUILD_NUMBER}")
file(WRITE ${HEADER_FILE} "#define qor_pp_buildnumber ${BUILD_NUMBER}")
