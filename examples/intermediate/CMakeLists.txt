add_subdirectory(client)
add_subdirectory(server)
if(CMAKE_SYSTEM_NAME MATCHES Windows)
#only on Windows at the moment
add_subdirectory(serialui)
add_subdirectory(terminal)
endif()
