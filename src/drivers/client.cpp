#include "shared.hpp"

#include <iostream>
#include <string>
#include <cstring>

void handle_client( network::connected_socket& connection )
{
	constexpr int buffer_size = 1024;
	char message[ buffer_size ] = "";

	while ( std::strcmp( message, "exit" ) && std::strcmp( message, "shutdown" ) )
	{
		std::cout << "Please enter a message to send the server: ";
		std::cin.getline( message, buffer_size, '\n' );

		connection.send( message, buffer_size );
		std::cout << "Sent the message: [" << message << "]\n";
		connection.recieve( message, std::strlen( message ) );
		std::cout << "Recieved the message: [" << message << "]\n";
	}
}

int main()
{
    try
	{
        using namespace network;
    
        // Create and connect client socket
        stream_socket my_socket( address_family, ip_protocol );
		socket_address addr( address_family, "localhost", "10000" );

		std::clog << "Queried DNS to get the \"localhost\" address of:\n" << addr << '\n';

        connected_socket client( my_socket.connect( addr ) );
        
		std::cout << "Connected to the server!\n";

		handle_client( client );
    }
    catch ( const network::socket_error& error )
	{
        std::cerr << "Error: " << error.what() << std::endl;
        return 1;
    }

    return 0;
}