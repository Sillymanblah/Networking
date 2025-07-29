#include "../network/socket"
#include "configs.hpp"

#include <iostream>
#include <string>
#include <cstring>

void handle_client( network::stream_socket& connection )
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
	using namespace network;

	try
	{
        // Create and connect client socket
        client_socket client( address_family, ip_protocol );

		std::clog << "Connecting to the server at the address:\n" << server_address << '\n';

        client.connect( server_address );
        
		std::cout << "Connected to the server!\n";

		handle_client( client );
    }
    catch ( const socket_error& error )
	{
        std::cerr << "Error: " << error.what() << std::endl;
        return 1;
    }

    return 0;
}