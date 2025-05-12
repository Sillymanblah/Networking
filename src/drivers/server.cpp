#include "../network/socket.hpp"

#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

constexpr int maximum_connections = 10;
constexpr network::AF address_family = network::AF::INET;
constexpr network::PF protocol_family = address_family;
constexpr network::PROTOCOL ip_protocol = network::PROTOCOL::TCP;

const network::socket_address server_address = network::socket_address( address_family, "localhost", "10000" );

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
	using namespace network;

	std::vector< std::thread > connection_handlers;

	try
	{
		server_socket server( address_family, ip_protocol );

		std::clog << "Queried the DNS to get the server address of:\n" << server_address << '\n';

		server.bind( server_address );

		server.listen( maximum_connections );

		while ( !shutdown )
		{
			connection_handlers.emplace_back( handle_connection, server.accept() );
		}
	}
	catch ( const std::runtime_error& error ) { std::cerr << error.what(); }

	// Join all the open threads as the connections are closed, we might want a way to forcibly close them.
	for ( std::thread& connection : connection_handlers )
		connection.join();

	return 0;
}