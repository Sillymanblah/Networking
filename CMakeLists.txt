cmake_minimum_required( VERSION 3.10 )
project( Server_Concurrency )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED true )

add_library(
	network STATIC
	src/network/socket.cpp
	src/network/socket_error.cpp
	src/network/socket_address.cpp
	src/network/codex.cpp
)

add_executable(
	server
	src/drivers/server.cpp
)

add_executable(
	client
	src/drivers/client.cpp
)

target_link_libraries( network Ws2_32 )
target_link_libraries( server network )
target_link_libraries( client network )