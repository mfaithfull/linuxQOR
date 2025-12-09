cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(kernel)
add_subdirectory(winsock2)
add_subdirectory(user)
add_subdirectory(gdi)