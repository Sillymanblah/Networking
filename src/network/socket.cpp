#include "socket.hpp"
#include <WS2tcpip.h>

namespace network
{
	// This is accessible to our program if we need to use it, but I imagine that would not be the case.
	static WSADATA data;

	unsigned short socket::count = 0;

	void socket::throw_error( int error_code )
	{ throw socket_error( error_code ); }

	void socket::throw_last_error()
	{ throw_error( WSAGetLastError() ); }
	
	void socket::initialize()
	{ if ( int code = WSAStartup( MAKEWORD( 2, 2 ), &data ) ) throw_error( code ); }

	void socket::deinitialize()
	{ if ( WSACleanup() ) throw_last_error(); }
	
	socket::socket( AF address_family, TYPE socket_type, PROTOCOL ip_protocol )
	{
		if ( !count ) initialize();
	
		this->socket_ptr = ::socket( (int) address_family, (int) socket_type, (int) ip_protocol );
	
		++count;
	}
	
	socket::~socket()
	{
		closesocket( this->socket_ptr );
		
		if ( !--count ) deinitialize();
	}

	void socket::set_flags( int flags )
	{ this->flags = flags; }
	int socket::get_flags()
	{ return this->flags; }

	int socket::recieve( char *buffer, int length )
	{ return ::recv( this->socket_ptr, buffer, length, this->flags ); }
	int socket::send( const char *buffer, int length )
	{ return ::send( this->socket_ptr, buffer, length, this->flags ); }

	
	void socket::recieve_all( char* buffer, int length )
	{
		int recieved = 0;
		do { recieved += this->recieve( buffer, length ); }
		while ( recieved != length );
	}
	void socket::send_all( const char* buffer, int length )
	{
		int sent = 0;
		do { sent += this->send( buffer, length ); }
		while ( sent != length );
		
	}

	union size_packet
	{
		size_t size;
		char raw[ sizeof( size_t ) ];
	};
	
	size_t socket::recieve_length()
	{
		size_packet packet;

		*this >> packet;

		return packet.size;
	}
	void socket::send_length( size_t length )
	{
		size_packet packet;

		packet.size = length;

		*this << packet;
	}
}
// namespace network
