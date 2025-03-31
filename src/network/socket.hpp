#pragma once
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "socket_error.hpp"
#include <string>

namespace network
{
	// Address Families
	enum class AF : int
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

	// Protocol Families are currently the same as Address Families
	typedef AF PF;

	// Protocols
	enum class PROTOCOL : int
	{
		HOPOPTS       = 0,  // IPv6 Hop-by-Hop options
		ICMP          = 1,
		IGMP          = 2,
		GGP           = 3,
		IPV4          = 4,
		ST            = 5,
		TCP           = 6,
		CBT           = 7,
		EGP           = 8,
		IGP           = 9,
		PUP           = 12,
		UDP           = 17,
		IDP           = 22,
		RDP           = 27,
		IPV6          = 41, // IPv6 header
		ROUTING       = 43, // IPv6 Routing header
		FRAGMENT      = 44, // IPv6 fragmentation header
		ESP           = 50, // encapsulating security payload
		AH            = 51, // authentication header
		ICMPV6        = 58, // ICMPv6
		NONE          = 59, // IPv6 no next header
		DSTOPTS       = 60, // IPv6 Destination options
		ND            = 77,
		ICLFXBM       = 78,
		PIM           = 103,
		PGM           = 113,
		L2TP          = 115,
		SCTP          = 132,
		RAW           = 255,
		MAX           = 256,
	};
	// enum class PROTOCOL
		
	// The `network::socket` class wraps WinSock, and eventually will wrap sys/socket as an easy and portable library.
	// Though, it is not recommended to use `network::socket` by itself, but instead one should use the two child classes: `network::connection` and `network::server`
	// Each has an overloaded constructor for the specific purpose you need and you do not need to worry about calls to `socket::bind`, `socket::connect`, etc.
	class socket
	{
	public:
		// Socket Types
		enum class TYPE : int
		{
			STREAM		= 1,		// stream socket
			DGRAM		= 2,		// datagram socket
			RAW			= 3,		// raw-protocol interface
			RDM			= 4,		// reliably-delivered message
			SEQPACKET	= 5,		// sequenced packet stream
		};
		// enum class TYPE

	private:
		// Track the number of active network handlers, when we hit 0, deinitialize WinSock.
		static unsigned short count;
		// Throws a `std::runtime_error` if the call to WSAStartup fails. If this 
		static void initialize();
		// Throws a `std::runtime_error` if the call to WSACleanup fails. If this happens, there are bigger issues at hand.
		static void deinitialize();

		// Throws an error with a message based on the respective cleanup error code, all other error codes are ignored.
		[[noreturn]] static void throw_error( int error_code );
		// Throws an error with a message based the last error retrieved by WSAGetLastError.
		[[noreturn]] static void throw_last_error();
		
	public:
		/*
			Creates a network socket handler, the default settings are for an IPv4 TCP socket stream, but that can be changed.
			It is important to note that when creating a `socket` object when there are no `socket` objects in existence yet, we must perform a
			call to WSAStartup, which might fail. in that event, a `std::runtime_error` will be thrown with an error message describing the issue.
		*/
		socket( AF address_family = AF::INET, TYPE socket_type = TYPE::STREAM, PROTOCOL ip_protocol = PROTOCOL::TCP );
		// Closes a network socket handler, and if it is the last one, also shuts down WSA
		~socket();

		// This is primitive, should find a better way to handle this/wrap this.
		void set_flags( int flags );
		int get_flags();

	public:
		// TODO: Standard socket wrapping functions.
		// TODO: Make address (IPv4, IPv6, and full address) and port structs
		void bind();
		void listen();
		socket accept();
		void connect();

	protected:
		// Standard socket `recv` function wrapper.
		inline int recieve( char *buffer, int length );
		// Standard socket `send` function wrapper.
		inline int send( const char *buffer, int length );

		// Recieves all data up to `length` into the `buffer` by repeatedly calling `socket::recieve`.
		inline void recieve_all( char* buffer, int length );
		// Sends all data up to `length` into the `buffer` by repeatedly calling `socket::send`.
		inline void send_all( const char* buffer, int length );

		// Recieves the expected size of an incoming string so we can reserve space.
		inline size_t recieve_length();
		// Sends the size of an outgoing string so the reciever can reserve space.
		inline void send_length( size_t length );
		
	public:
		// The following are stream operators to pull and push data through the network, using unions.
		// Those unions are expected to have a character array by the name of `raw` which is the same size as the struct storing the ordered data.

		template < class _Data >
		socket& operator << ( const _Data& data )
		{
			send_all( data.raw, sizeof( data.raw ) );
			return *this;
		}
		template < class _Data >
		socket& operator << ( _Data&& data )
		{
			send_all( data.raw, sizeof( data.raw ) );
			return *this;
		}
		template < class _Data >
		socket& operator >> ( _Data& data )
		{
			recieve_all( data.raw, sizeof( data.raw ) );
			return *this;
		}
		
		// The following are stream operators to pull and push data through the network on standard strings, resizing as needed.

		template < class _Char >
		socket& operator << ( const std::basic_string< _Char >& data )
		{
			send_length( data.length() );
			send_all( data.c_str(), data.length() );
			return *this;
		}
		template < class _Char >
		socket& operator << ( std::basic_string< _Char >&& data )
		{
			send_length( data.length() );
			send_all( data.c_str(), data.length() );
			return *this;
		}
		template < class _Char >
		socket& operator >> ( std::basic_string< _Char >& data )
		{
			size_t incoming = recieve_length();
			data.reserve( incoming );
			recieve_all( data.data(), incoming );
			return *this;
		}
		
	protected:
		unsigned long long socket_ptr;
		int flags;
	};
}
// namespace network

#endif // SOCKET_HPP