#pragma once
#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include <string>
#include <stdint.h>
#include <array>

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

		// Convert string(s) to socket_address via direct conversion or DNS query, while specifying ipv4 or ipv6
		// This function will not work yet and instead always throws an exception.
		socket_address( AF address_family, std::string address );
		// Convert string(s) to socket_address via direct conversion or DNS query, while specifying ipv4 or ipv6
		// port can be a service name or a number.
		socket_address( AF address_family, std::string address, std::string port );

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