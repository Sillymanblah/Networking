#pragma once
#ifndef ADDRESS_HPP
#define ADDRESS_HPP

// Temporary include
#include "socket_error.hpp"

#include <string>
#include <cstdint>
#include <array>
#include <ostream>
#include <istream>

namespace network
{
	// Address Families
	enum class AF : uint16_t
	{
		UNSPEC		= 0,		// unspecified
		UNIX		= 1,		// local to host (pipes, portals)
		INET		= 2,		// internetwork: UDP, TCP, etc.
		IMPLINK		= 3,		// arpanet imp addresses
		PUP			= 4,		// pup protocols: e.g. BSP
		CHAOS		= 5,		// mit CHAOS protocols
		NS			= 6,		// XEROX NS protocols
		IPX			= NS,		// IPX protocols: IPX, SPX, etc.
		ISO			= 7,		// ISO protocols
		OSI			= ISO,	// OSI is ISO
		ECMA		= 8,		// european computer manufacturers
		DATAKIT		= 9,		// datakit protocols
		CCITT		= 10,		// CCITT protocols, X.25 etc
		SNA			= 11,		// IBM SNA
		DECnet		= 12,		// DECnet
		DLI			= 13,		// Direct data link interface
		LAT			= 14,		// LAT
		HYLINK		= 15,		// NSC Hyperchannel
		APPLETALK	= 16,		// AppleTalk
		NETBIOS		= 17,		// NetBios-style addresses
		VOICEVIEW	= 18,		// VoiceView
		FIREFOX		= 19,		// Protocols from Firefox
		UNKNOWN1	= 20,		// Somebody is using this!
		BAN			= 21,		// Banyan
		ATM			= 22,		// Native ATM Services
		INET6		= 23,		// Internetwork Version 6
		CLUSTER		= 24,		// Microsoft Wolfpack
		IEEE_1284	= 25,		// IEEE 1284.4 WG AF
		IRDA		= 26,		// IrDA
		NETDES		= 28,		// Network Designers OSI & gateway
	};
	// enum class AF

	class socket_address
	{
	public:
		// IPv4 constructor
		socket_address( AF address_family, std::array< uint8_t, 4 > octets, uint16_t port = 2027 );
		// IPv4 constructor
		socket_address( AF address_family, uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4, uint16_t port = 2027 );

		// IPv6 constructor
		socket_address( AF address_family, std::array< uint16_t, 8 > hextets, uint16_t port = 2027 );
		// IPv6 constructor
		socket_address( AF address_family, uint16_t hextet1, uint16_t hextet2, uint16_t hextet3, uint16_t hextet4, uint16_t hextet5, uint16_t hextet6, uint16_t hextet7, uint16_t hextet8, uint16_t port = 2027 );

	private:
		void build_address( AF address_family, const std::string& address, const std::string& port  );

	public:
		// Convert string(s) to socket_address via direct conversion or DNS query, while specifying ipv4 or ipv6
		// This function will not work yet and instead always throws an exception.
		socket_address( AF address_family, const std::string& address );
		// Convert string(s) to socket_address via direct conversion or DNS query, while specifying ipv4 or ipv6
		// port can be a service name or a number.
		socket_address( AF address_family, const std::string& address, const std::string& port );

		// IPv6 data structure 
		struct IPv6_address
		{
			// Structs and unions for easier usage.
			struct unicast_data
			{
				struct prefix_data
				{
					uint64_t routing : 48;
					uint64_t subnet : 16;
				};

				union
				{
					prefix_data prefix;
					uint8_t bytes[8];
					uint16_t hextets[4];
				} network;

				union
				{
					uint64_t interface;
					uint8_t bytes[8];
					uint16_t hextets[4];
				} node;
			};

			union address_data
			{
				uint8_t bytes[16];
				uint16_t hextets[8];
				unicast_data unicast;
			};

			// Actual structure of the datatype.
			AF address_family;
			uint16_t port;
			uint32_t flow_info; // Currently unutilized by the system.
			address_data address;
			uint32_t scope_id; // Currently unutilized by the system.
		};

		struct IPv4_address
		{
			// Structs and unions for easier usage.
			struct subnetting_address
			{
				uint32_t network_prefix : 24;
				uint32_t host_id : 8;
			};

			union address_data
			{
				uint8_t octets[4];
				uint32_t raw;
				subnetting_address subnet;
			};

			// Actual structure of the datatype.
			AF address_family;
			uint16_t port;
			address_data address;
			uint8_t padding[ 8 ];
		};

	public:
		bool is_ipv6_address() const;

		bool is_ipv4_address() const;

		IPv4_address get_ipv4() const;

		IPv6_address get_ipv6() const;

	private:
		template < class _Elem, class _Traits >
		void print_ipv4( std::basic_ostream< _Elem, _Traits >& output ) const
		{
			// State of the output
			std::ios_base::iostate state = std::ios_base::goodbit;

			// Setting the format flags basefield to decimal without showing the base for this print operation.
			std::ios_base::fmtflags previous = output.setf( std::ios_base::dec, std::ios_base::basefield | std::ios_base::showbase );

			// Getting the num_put facet for the locale.
			using num_put = std::num_put< _Elem >;
			const num_put& num_facet = std::use_facet< num_put >( output.getloc() );

			for ( size_t index = 0; index < 4 && state == std::ios_base::goodbit; ++index )
			{
				// For all iterations after the first, put a dot between the numbers, if this fails, set the badbit.
				if ( index != 0 && output.rdbuf()->sputc( '.' ) == _Traits::eof() )
					state = std::ios_base::badbit;

				// If there was no failure in the put, then if the put operation for the number fails, set the badbit.
				else if ( num_facet.put( output, output, output.fill(), static_cast< uint64_t >( this->data.ipv4.address.octets[ index ] ) ).failed() )
					state = std::ios_base::badbit;
			}

			// Returning the basefield to the previous setting from before we changed it.
			output.setf( previous, std::ios_base::basefield | std::ios_base::showbase );

			// Set the state based on our current state.
			output.setstate( state );
		}

		// Does not currently implement the rules for omitting zeroes by use of the double colon.
		template < class _Elem, class _Traits >
		void print_ipv6( std::basic_ostream< _Elem, _Traits >& output ) const
		{
			// State of the output
			std::ios_base::iostate state = std::ios_base::goodbit;

			// Setting the format flags basefield to hexidecimal without showing the base for this print operation.
			std::ios_base::fmtflags previous = output.setf( std::ios_base::hex, std::ios_base::basefield | std::ios_base::showbase );

			// Getting the num_put facet for the locale.
			using num_put = std::num_put< _Elem >;
			const num_put& num_facet = std::use_facet< num_put >( output.getloc() );

			// Begin the output of a ipv6 address with the opening bracket, if this failes, set the badbit immediately.
			if ( output.rdbuf()->sputc( '[' ) == _Traits::eof() )
				state = std::ios_base::badbit;

			for ( size_t index = 0; index < 8 && state == std::ios_base::goodbit; ++index )
			{
				// For all iterations after the first, put a colon between the numbers, if this fails, set the badbit.
				if ( index != 0 && output.rdbuf()->sputc( ':' ) == _Traits::eof() )
					state = std::ios_base::badbit;

				// If there was no failure in the put, then if the put operation for the number fails, set the badbit.
				else if ( num_facet.put( output, output, output.fill(), static_cast< uint64_t >( this->data.ipv6.address.hextets[ index ] ) ).failed() )
					state = std::ios_base::badbit;
			}

			// If state is still good, output the closing bracket and we can get on with life.
			if ( state == std::ios_base::goodbit && output.rdbuf()->sputc( ']' ) == _Traits::eof() )
				state = std::ios_base::badbit;

			// Returning the basefield to the previous setting from before we changed it.
			output.setf( previous, std::ios_base::basefield | std::ios_base::showbase );

			// Set the state based on our current state.
			output.setstate( state );
		}

		static uint16_t fix_port( const uint16_t& port );

		template < class _Elem, class _Traits >
		void print_port( std::basic_ostream< _Elem, _Traits >& output ) const
		{
			// State of the output
			std::ios_base::iostate state = std::ios_base::goodbit;

			// Setting the format flags basefield to decimal without showing the base for this print operation.
			std::ios_base::fmtflags previous = output.setf( std::ios_base::dec, std::ios_base::basefield | std::ios_base::showbase );

			// Getting the num_put facet for the locale.
			using num_put = std::num_put< _Elem >;
			const num_put& num_facet = std::use_facet< num_put >( output.getloc() );

			// For all iterations after the first, put a colon between the numbers, if this fails, set the badbit.
			if ( output.rdbuf()->sputc( ':' ) == _Traits::eof() )
				state = std::ios_base::badbit;

			// If there was no failure in the put, then if the put operation for the number fails, set the badbit.
			// Note that technically both ports should be in the same place, but this might be compiler dependent, so to avoid that we check ahead of time.
			else if ( num_facet.put( output, output, output.fill(), static_cast< uint64_t >( fix_port( this->is_ipv6 ? this->data.ipv6.port : this->data.ipv4.port ) ) ).failed() )
				state = std::ios_base::badbit;

			// Returning the basefield to the previous setting from before we changed it.
			output.setf( previous, std::ios_base::basefield | std::ios_base::showbase );

			// Set the state based on our current state.
			output.setstate( state );
		}

	public:
		template < class _Elem, class _Traits >
		friend std::basic_ostream< _Elem, _Traits >& operator << ( std::basic_ostream< _Elem, _Traits >& output, const socket_address& address )
		{
			typename std::basic_ostream< _Elem, _Traits >::sentry my_sentry( output );

			if ( my_sentry )
			{
				if ( address.is_ipv6 ) address.print_ipv6( output );
				else address.print_ipv4( output );

				// If the output state is good after printing out the ip base, print the port.
				if ( output.good() ) address.print_port( output );
			}

			return output;
		}

		template < class _Elem, class _Traits >
		friend std::basic_istream< _Elem, _Traits >& operator >> ( std::basic_istream< _Elem, _Traits >& input, const socket_address& address )
		{ throw socket_error( socket_error::UNIMPLEMENTED ); }

	private:
		union
		{
			IPv4_address ipv4;
			IPv6_address ipv6;
		} data;

		bool is_ipv6;
	};
}
// namespace network

#endif // ADDRESS_HPP