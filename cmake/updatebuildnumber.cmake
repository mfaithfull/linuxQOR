# Define paths
set(CACHE_FILE "${CMAKE_CURRENT_BINARY_DIR}/generated/build_number_cache.txt")
set(HEADER_FILE "${CMAKE_CURRENT_BINARY_DIR}/generated/buildnumber.h")

add_custom_target(
    ${target}_buildnumber    
    COMMAND ${CMAKE_COMMAND}
    -DHEADER_FILE=${HEADER_FILE}
    -DCACHE_FILE=${CACHE_FILE}
    -P "${CMAKE_SOURCE_DIR}/cmake/buildnumber.cmake"
)
add_dependencies(${target} ${target}_buildnumber)

# Include generated header
target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated")

   