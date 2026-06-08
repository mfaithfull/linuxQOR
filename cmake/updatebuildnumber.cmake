# Define paths
set(CACHE_FILE "${CMAKE_CURRENT_BINARY_DIR}/generated/build_number_cache.txt")
set(HEADER_FILE "${CMAKE_CURRENT_BINARY_DIR}/generated/buildnumber.h")

# Run script before build
add_custom_command(
    TARGET ${target}
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND}
    -DHEADER_FILE=${HEADER_FILE}
    -DCACHE_FILE=${CACHE_FILE}
    -P "${CMAKE_SOURCE_DIR}/cmake/buildnumber.cmake"
)

# Include generated header
target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated")
