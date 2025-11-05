if(WIN32)
add_custom_command(TARGET ${target} POST_BUILD
  COMMAND ${CMAKE_COMMAND} 
  -Dsrc_dir="${CMAKE_CURRENT_BINARY_DIR}" 
  -Ddst_dir="${CMAKE_SOURCE_DIR}/cmakebuild/test"
    -P "${CMAKE_SOURCE_DIR}/cmake/update_directory.cmake"
)
add_custom_command(TARGET ${target} POST_BUILD
  COMMAND ${CMAKE_COMMAND} 
  -Dsrc_dir="${CMAKE_CURRENT_BINARY_DIR}" 
  -Ddst_dir="${CMAKE_SOURCE_DIR}/cmakebuild/examples/echoserver"
    -P "${CMAKE_SOURCE_DIR}/cmake/update_directory.cmake"
)
endif()