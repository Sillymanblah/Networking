cmake_minimum_required( VERSION 3.10 )
project( Server_Concurrency )

set( CMAKE_CXX_STANDARD_REQUIRED 20 )

add_executable(
	server
	src/drivers/server.cpp
)

add_library(
	network STATIC
	src/network/socket.cpp
	src/network/socket_error.cpp
)

target_link_libraries( network Ws2_32 )
target_link_libraries( server Ws2_32 network )