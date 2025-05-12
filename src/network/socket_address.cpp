#include "socket_address.hpp"
#include "socket_error.hpp"

#include <ws2tcpip.h>
#include <stdexcept>

// temporary includes for testing
#include <iostream>

namespace network
{
	socket_address::socket_address( AF address_family, std::array< uint8_t, 4 > octets, uint16_t port ) : is_ipv6( false )
	{
		this->data.ipv4.address_family = address_family;
		this->data.ipv4.port = htons( port );
		memcpy( this->data.ipv4.address.octets, octets.data(), 4 );
	}

	socket_address::socket_address( AF address_family, uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4, uint16_t port ) :
		socket_address( address_family, std::array< uint8_t, 4 >{ octet1, octet2, octet3, octet4 }, port ) {}

	socket_address::socket_address( AF address_family, std::array< uint16_t, 8 > hextets, uint16_t port ) : is_ipv6( true )
	{
		this->data.ipv6.address_family = address_family;
		this->data.ipv6.port = htons( port );
		memcpy( this->data.ipv6.address.hextets, hextets.data(), 8 );
	}

	socket_address::socket_address( AF address_family, uint16_t hextet1, uint16_t hextet2, uint16_t hextet3, uint16_t hextet4, uint16_t hextet5, uint16_t hextet6, uint16_t hextet7, uint16_t hextet8, uint16_t port ) :
		socket_address( address_family, std::array< uint16_t, 8 >{ hextet1, hextet2, hextet3, hextet4, hextet5, hextet6, hextet7, hextet8 }, port ) {}


	void socket_address::build_address( AF address_family, const std::string& address, const std::string& port )
	{
		if ( address.empty() ) throw socket_error( socket_error::ADDRESS_NOT_FOUND );

		// Need to read data out of address and port.
		addrinfo* addresses;
		addrinfo hints {};
		hints.ai_family = static_cast< int > ( address_family );

		// Need to read data out of address and port.
		if ( getaddrinfo( address.c_str(), port.c_str(), &hints, &addresses ) ) throw socket_error( WSAGetLastError() );

		if ( addresses == nullptr ) throw socket_error( socket_error::ADDRESS_NOT_FOUND );

		if ( addresses->ai_addrlen == sizeof( IPv4_address ) )
		{
			memcpy( &this->data.ipv4, addresses->ai_addr, sizeof( IPv4_address ) );
			is_ipv6 = false;
		}
		else if ( addresses->ai_addrlen == sizeof( IPv6_address ) )
		{
			memcpy( &this->data.ipv6, addresses->ai_addr, sizeof( IPv6_address ) );
			is_ipv6 = true;
		}
		else throw socket_error( socket_error::BAD_ADDRESS_TYPE ); // Address type unable to be deduced, throw an error.
	}

	socket_address::socket_address( AF address_family, const std::string& address )
	{ throw socket_error( socket_error::UNIMPLEMENTED ); }

	socket_address::socket_address( AF address_family, const std::string& address, const std::string& port )
	{
		// Temporary workaround for the fact that this needs WSA to be started up before we can use it, we need to use WSAStartup and Cleanup in here for now...
		WSAData data;
		::WSAStartup( MAKEWORD( 2, 2 ), &data );

		try { this->build_address( address_family, address, port ); } // Attempt to build the address.
		catch ( ... )
		{
			::WSACleanup(); // Clean up WSA on failed build.
			throw; // Rethrow the previous error.
		}

		::WSACleanup(); // Cleanup WSA on successful address build.
	}

	bool socket_address::is_ipv6_address() const
	{ return this->is_ipv6; }

	bool socket_address::is_ipv4_address() const
	{ return !this->is_ipv6; }

	socket_address::IPv4_address socket_address::get_ipv4() const
	{
		if ( this->is_ipv6 ) throw socket_error( socket_error::BAD_ADDRESS_TYPE );
		return this->data.ipv4;
	}
	
	socket_address::IPv6_address socket_address::get_ipv6() const
	{
		if ( !this->is_ipv6 ) throw socket_error( socket_error::BAD_ADDRESS_TYPE );
		return this->data.ipv6;
	}

	uint16_t socket_address::fix_port( const uint16_t& port )
	{ return ntohs( port ); }
}
// namespace network
