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

	if ( !shutdown && std::strcmp( message, "shutdown" ) == 0 )
	{
		shutdown = true;
		network::client_socket ping( address_family, ip_protocol );
		ping.connect( server_address ); // Ping our own server, so we can stop listening.
	}
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

		do
		{
			std::clog << "Checking for connections...\n";

			// Note that `connection_waiting` may or may not be a blocking call depending on the system, though from testing it appears to block until there is a connection waiting, so it might be more apt to call it `await_connection`.
			if ( server.connection_waiting() && !shutdown )
			{
				try
				{
					stream_socket new_connection( server.accept() );
					std::clog << "Accepted the connection!\nPassing it on to the handler...\n";
					connection_handlers.emplace_back( handle_connection, std::move( new_connection ) );
				}
				catch ( const socket_error& err ) {} // Do nothing the only error this should be is a failed accept operation that we can ignore.
			}
			// In the event that `connection_waiting` is non-blocking on the socket for some reason, which would mean someone used a hacky workaround to implant their own `socket_ptr` into `server_socket`, there is a catch here.
			else
			{
				std::this_thread::sleep_for( std::chrono::seconds( 1 ) ); // Server waits a moment so we don't burn a million resources constantly checking if there is a connection.
				std::clog << "There were no connections to accept...\n";
			}
		}
		while ( !shutdown );
	}
	catch ( const std::runtime_error& error ) { std::cerr << error.what(); }

	// Join all the open threads as the connections are closed, we might want a way to forcibly close them.
	for ( std::thread& connection : connection_handlers )
		connection.join();

	return 0;
}