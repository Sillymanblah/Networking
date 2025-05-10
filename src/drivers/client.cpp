#include "../network/socket.hpp"
#include <iostream>
#include <string>
#include <cstring>

void handle_client( network::stream_socket& connection )
{
	constexpr int buffer_size = 1024;
	char message[ buffer_size ] = "";

	while ( std::strcmp( message, "exit" ) )
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

        // Set up connection parameters
        constexpr AF address_family = AF::INET;
        constexpr PROTOCOL ip_protocol = PROTOCOL::TCP;
        std::array< uint8_t, 4 > local_host{ 127, 0, 0, 1 };
        uint16_t port = 8080;
    
        // Create and connect client socket
        client_socket client( address_family, ip_protocol );
        client.connect( socket_address( address_family, local_host, port ) );
        
		std::cout << "Connected to the server!\n";

		handle_client( client );
    }
    catch ( const std::exception& error )
	{
        std::cerr << "Error: " << error.what() << std::endl;
        return 1;
    }

    return 0;
}