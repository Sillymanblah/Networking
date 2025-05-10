#include "../network/socket.hpp"

#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

bool shutdown = false;

void handle_connection( network::stream_socket connection )
{
	// Handle client connection
	std::cout << "New client connected!" << std::endl;
	// Perform operations with the client socket...

	constexpr int buffer_size = 1024;
	char message[ buffer_size ] = "";

	while ( std::strcmp( message, "exit" ) && std::strcmp( message, "shutdown" ) )
	{
		connection.recieve( message, buffer_size );
		std::cout << "Received message: [" << message << "]\n";
		connection.send( message, std::strlen( message ) );
		std::cout << "Returned message: [" << message << "]\n";
	}

	if ( std::strcmp( message, "shutdown" ) == 0 ) shutdown = true;
}

int main()
{
	std::cout << "Hello, World!\n";
	try
	{
		using namespace network;

		constexpr int maximum_connections = 10;
		constexpr AF address_family = AF::INET;
		constexpr PF protocol_family = address_family;
		constexpr PROTOCOL ip_protocol = PROTOCOL::TCP;

		std::array< uint8_t, 4 > local_host{ 127, 0, 0, 1 };
		uint16_t port = 8080;

		server_socket server( address_family, ip_protocol );
		server.bind( socket_address( address_family, local_host, port ) );

		server.listen( maximum_connections );
		;

		std::vector< std::thread > connection_handlers;

		while ( !shutdown )
		{
			connection_handlers.emplace_back( handle_connection, server.accept() );
		}
	}
	catch ( const std::runtime_error& error ) { std::cerr << error.what(); }

	std::cout << "Goodbye, World!\n";
	
	return 0;
}