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

	socket_base::~socket_base()
	{ destroy_socket( this->pointer ); }
	
	void socket_base::assert_valid() const
	{ if ( this->pointer == INVALID_SOCKET ) throw_error( socket_error::SOCKET_NOT_VALID ); }

	// Constructor that takes an address family and protocol to create a socket for use.
	basic_socket< SOCKET_TYPE::STREAM >::basic_socket( AF address_family, PROTOCOL ip_protocol ) :
		socket_base( address_family, SOCKET_TYPE::STREAM, ip_protocol ), flags( 0 ) {}

	// Move constructor copies the data and deletes the old object's data.
	basic_socket< SOCKET_TYPE::STREAM >::basic_socket( basic_socket&& other ) : socket_base( std::move( other ) ), flags( 0 ) {}

	// Move assignment copies the data and deletes the old object's data.
	basic_socket< SOCKET_TYPE::STREAM >& basic_socket< SOCKET_TYPE::STREAM >::operator = ( basic_socket&& other )
	{
		( socket_base& ) *this = std::move( other );
		this->flags = other.flags;
		return *this;
	}

	basic_socket< SOCKET_TYPE::STREAM >::basic_socket( socket_ptr socket ) : socket_base( socket ) {}

	union size_packet
	{
		size_t size;
		char raw[ sizeof( size_t ) ];
	};

	int basic_socket< SOCKET_TYPE::STREAM >::recieve( char *buffer, int length )
	{
		this->assert_valid();

		int recieved = ::recv( this->pointer, buffer, length, this->flags );
		if ( recieved == SOCKET_ERROR ) throw_last_error();

		return recieved;
	}

	int basic_socket< SOCKET_TYPE::STREAM >::send( const char *buffer, int length )
	{
		this->assert_valid();

		int sent = ::send( this->pointer, buffer, length, this->flags );
		if ( sent == SOCKET_ERROR ) throw_last_error();

		return sent;
	}

	void basic_socket< SOCKET_TYPE::STREAM >::recieve_all( char* buffer, int length )
	{ for ( int recieved = 0; recieved != length; recieved += this->recieve( buffer, length ) ); }

	void basic_socket< SOCKET_TYPE::STREAM >::send_all( const char* buffer, int length )
	{ for ( int sent = 0; sent != length; sent += this->send( buffer, length ) ); }


	// Constructor that takes an address family and protocol to create a socket for use.
	client_socket::client_socket( AF address_family, PROTOCOL ip_protocol ) : basic_socket< SOCKET_TYPE::STREAM >( address_family, ip_protocol ) {}

	// Move constructor copies the data and deletes the old object's data.
	client_socket::client_socket( client_socket&& other ) : basic_socket< SOCKET_TYPE::STREAM >( std::move( other ) ) {}

	// Move assignment copies the data and deletes the old object's data.
	client_socket& client_socket::operator = ( client_socket&& other )
	{
		( basic_socket< SOCKET_TYPE::STREAM >& ) *this = std::move( other );
		return *this;
	}

	void client_socket::connect( const socket_address& address )
	{
		this->assert_valid();

		// Might want to check that we are not already connected.
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

	// Constructor that takes an address family and protocol to create a socket for use.
	server_socket::server_socket( AF address_family, PROTOCOL ip_protocol ) : socket_base( address_family, SOCKET_TYPE::STREAM, ip_protocol ) {}

	// Move constructor copies the data and deletes the old object's data.
	server_socket::server_socket( server_socket&& other ) : socket_base( std::move( other ) ) {}

	// Move assignment copies the data and deletes the old object's data.
	server_socket& server_socket::operator = ( server_socket&& other )
	{
		( socket_base& ) *this = std::move( other );
		return *this;
	}

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
	
	void server_socket::listen( int backlog )
	{
		this->assert_valid();
		// Might want to check that we are bound to an address.

		if ( ::listen( this->pointer, backlog ) == SOCKET_ERROR ) throw_last_error();
	}

	bool server_socket::connection_waiting()
	{
		fd_set my_socket{ 1, { this->pointer } };

		int result = ::select( 0, &my_socket, nullptr, nullptr, nullptr );
		
		if ( result == SOCKET_ERROR ) throw_last_error();

		return result;
	}

	stream_socket server_socket::accept()
	{
		this->assert_valid();
		// Might want to check that we are in listening mode.

		socket_ptr socket = ::accept( this->pointer, nullptr, nullptr );
		if ( socket == INVALID_SOCKET ) throw_last_error();

		return socket;
	}
}
// namespace network
