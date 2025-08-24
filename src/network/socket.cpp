#include "socket.hpp"
#include "socket_error.hpp"

#include <ws2tcpip.h>

namespace network
{
	// This is accessible to our program if we need to use it, but I imagine that would not be the case.
	static WSADATA data;

	unsigned short socket_base::count = 0;

	void socket_base::throw_error( int error_code )
	{ throw socket_error( error_code ); }

	void socket_base::throw_last_error()
	{ throw_error( WSAGetLastError() ); }

	void socket_base::initialize()
	{ if ( int code = WSAStartup( MAKEWORD( 2, 2 ), &network::data ) ) throw_error( code ); }

	void socket_base::deinitialize()
	{ if ( WSACleanup() ) throw_last_error(); }

	socket_base::socket_base() : pointer( INVALID_SOCKET ) {}

	socket_base::socket_ptr socket_base::create_socket( AF address_family, SOCKET_TYPE type, PROTOCOL ip_protocol )
	{
		if ( !count ) initialize();

		socket_ptr new_socket = ::socket( ( int ) address_family, ( int ) type, ( int ) ip_protocol );

		if ( new_socket == INVALID_SOCKET ) throw_last_error();

		++count;

		return new_socket;
	}

	socket_base::socket_base( AF address_family, SOCKET_TYPE type, PROTOCOL ip_protocol ) :
		pointer( create_socket( address_family, type, ip_protocol ) ) {}

	socket_base::socket_base( socket_ptr socket, int flags ) : pointer( socket )
	{ ++count; }

	void socket_base::clear()
	{ this->pointer = INVALID_SOCKET; }

	socket_base::socket_base( socket_base&& other ) : pointer( other.pointer )
	{ other.clear(); }

	void socket_base::destroy_socket( socket_ptr socket )
	{
		if ( socket == INVALID_SOCKET ) return; // Nothing to do
		if ( ::closesocket( socket ) == SOCKET_ERROR ) throw_last_error();
		if ( !--count ) deinitialize();
	}

	socket_base& socket_base::operator = ( socket_base&& other )
	{
		destroy_socket( this->pointer );

		this->pointer = other.pointer;
		other.clear();
		return *this;
	}

	void socket_base::close()
	{
		destroy_socket( this->pointer );
		this->clear();
	}

	socket_base::~socket_base()
	{ destroy_socket( this->pointer ); }

	void socket_base::assert_valid() const
	{ if ( this->pointer == INVALID_SOCKET ) throw_error( socket_error::SOCKET_NOT_VALID ); }

	basic_socket< SOCKET_TYPE::STREAM >::basic_socket( socket_ptr pointer, int flags )  : socket_base( pointer ), flags( flags ) {}

	// Constructor that takes an address family and protocol to create a socket for use.
	basic_socket< SOCKET_TYPE::STREAM >::basic_socket( AF address_family, PROTOCOL ip_protocol ) :
		socket_base( address_family, SOCKET_TYPE::STREAM, ip_protocol ), flags( 0 ) {}

	stream_socket::stream_socket( AF address_family, PROTOCOL ip_protocol ) : stream_socket_base( address_family, ip_protocol ) {}

	connected_socket stream_socket::connect( const socket_address& address ) { return connected_socket( std::move( *this ), address ); }
	
	server_socket stream_socket::open( const socket_address& address, int backlog ) { return server_socket( std::move( *this ), address, backlog ); }

	void server_socket::bind( const socket_address& address )
	{
		this->assert_valid();

		if ( address.is_ipv6_address() )
		{
			socket_address::IPv6_address ipv6 = address.get_ipv6();
			if ( ::bind( this->pointer, ( sockaddr* ) &ipv6, sizeof( socket_address::IPv6_address ) ) == SOCKET_ERROR ) throw_last_error();
		}
		else
		{
			socket_address::IPv4_address ipv4 = address.get_ipv4();
			if ( ::bind( this->pointer, ( sockaddr* ) &ipv4, sizeof( socket_address::IPv4_address ) ) == SOCKET_ERROR ) throw_last_error();
		}
	}

	void server_socket::listen( int backlog ) { if ( ::listen( this->pointer, backlog ) == SOCKET_ERROR ) throw_last_error(); }

	server_socket::server_socket( stream_socket&& other, const socket_address& address, int backlog ) : stream_socket_base( std::move( other ) )
	{
		this->bind( address );
		this->listen( backlog );
	}

	server_socket::server_socket( AF address_family, PROTOCOL ip_protocol, const socket_address& address, int backlog ) : stream_socket_base( address_family, ip_protocol )
	{
		this->bind( address );
		this->listen( backlog );
	}

	bool server_socket::connection_waiting()
	{
		fd_set my_socket{ 1, { this->pointer } };

		int result = ::select( 0, &my_socket, nullptr, nullptr, nullptr );

		if ( result == SOCKET_ERROR ) throw_last_error();

		return result;
	}

	connected_socket server_socket::accept()
	{
		socket_ptr socket = ::accept( this->pointer, nullptr, nullptr );
		if ( socket == INVALID_SOCKET ) throw_last_error();

		return connected_socket( socket, this->flags );
	}

	void connected_socket::connect( const socket_address& address )
	{
		if ( address.is_ipv6_address() )
		{
			socket_address::IPv6_address ipv6 = address.get_ipv6();
			if ( ::connect( this->pointer, ( sockaddr* ) &ipv6, sizeof( socket_address::IPv6_address ) ) == SOCKET_ERROR ) throw_last_error();
		}
		else
		{
			socket_address::IPv4_address ipv4 = address.get_ipv4();
			if ( ::connect( this->pointer, ( sockaddr* ) &ipv4, sizeof( socket_address::IPv4_address ) ) == SOCKET_ERROR ) throw_last_error();
		}
	}

	connected_socket::connected_socket( stream_socket&& other, const socket_address& address ) : stream_socket_base( std::move( other ) ) { this->connect( address ); }

	connected_socket::connected_socket( socket_ptr pointer, int flags ) : stream_socket_base( pointer, flags ) {}
	
	connected_socket::connected_socket( AF address_family, PROTOCOL ip_protocol, const socket_address& address ) : stream_socket_base( address_family, ip_protocol ) { this->connect( address ); }

	int connected_socket::recieve( char *buffer, int length )
	{
		int recieved = ::recv( this->pointer, buffer, length, this->flags );
		if ( recieved == SOCKET_ERROR ) throw_last_error();

		return recieved;
	}

	int connected_socket::send( const char *buffer, int length )
	{
		int sent = ::send( this->pointer, buffer, length, this->flags );
		if ( sent == SOCKET_ERROR ) throw_last_error();

		return sent;
	}

	void connected_socket::recieve_all( char* buffer, int length )
	{ for ( int recieved = 0; recieved != length; recieved += this->recieve( buffer, length ) ); }

	void connected_socket::send_all( const char* buffer, int length )
	{ for ( int sent = 0; sent != length; sent += this->send( buffer, length ) ); }

	void connected_socket::disconnect() { this->close(); }
}
// namespace network
