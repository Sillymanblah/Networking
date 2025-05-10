#pragma once
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "socket_address.hpp"

#include <string>

namespace network
{
	// Protocol Families are currently the same as Address Families
	typedef AF PF;

	// Protocols, as seen in winsock2.h.
	enum class PROTOCOL : uint8_t
	{
		HOPOPTS		= 0, // IPv6 Hop-by-Hop options
		ICMP		= 1, // Internet Control Message Protocol, version 4
		IGMP		= 2, // Internet Group Management Protocol, version 4
		GGP			= 3, // Gateway-to-Gateway Protocol, version 4
		IPV4		= 4, // Internet Protocol, version 4
		ST			= 5, // Internet Stream Protocol
		TCP			= 6, // Transmission Control Protocol
		CBT			= 7, // Core-Based Trees Protocol
		EGP			= 8, // Exterior Gateway Protocol
		IGP			= 9, // Interior Gateway Protocol
		PUP			= 12, // PARC Universal Packet Protocol
		UDP			= 17, // User Datagram protocol
		IDP			= 22, // Internet Datagram Protocol
		RDP			= 27, // Remote Desktop Protocol
		IPV6		= 41, // Internet Protocol, version 6
		ROUTING		= 43, // IPv6 Routing Header
		FRAGMENT	= 44, // IPv6 Fragmentation Header
		ESP			= 50, // Encapsulating Security Payload
		AH			= 51, // Authentication Header
		ICMPV6		= 58, // Internet Control Message Protocol, version 6
		NONE		= 59, // IPv6 No Next Header
		DST_OPTS	= 60, // IPv6 Destination Options
		NDP			= 77, // Neighbor Discovery Protocol
		ICLFXBM		= 78, // Not a damn clue what this is, but windows supports it.
		PIM			= 103, // Protocol Independent Multicast
		PGM			= 113, // Pragmatic General Multicast
		L2TP		= 115, // Layer 2 Tunneling Protocol
		SCTP		= 132, // Stream Control Transmission Protocol
		RAW			= 255, // Raw IP packets
	};

	// Socket Types
	enum class SOCKET_TYPE : uint8_t
	{
		STREAM		= 1,	// stream socket
		DGRAM		= 2,	// datagram socket
		RAW			= 3,	// raw-protocol interface
		RDM			= 4,	// reliably-delivered message
		SEQPACKET	= 5,	// sequenced packet stream
	};
	
	class socket_base
	{
	protected:
		// Type alias equivalent to SOCKET in WinSock
		using socket_ptr	= uint64_t;

	private:
		// Track the number of active network handlers, when we hit 0, deinitialize WinSock.
		static unsigned short count;
	
	protected:
		// Throws an error with a message based on the respective cleanup error code, all other error codes are ignored.
		[[noreturn]] static void throw_error( int error_code );
		// Throws an error with a message based the last error retrieved by WSAGetLastError.
		[[noreturn]] static void throw_last_error();

	private:
		// Throws a `std::runtime_error` if the call to WSAStartup fails. If this happens, no socket is created and network communications cannot be established.
		static void initialize();
		// Throws a `std::runtime_error` if the call to WSACleanup fails. If this happens, there are bigger issues at hand.
		static void deinitialize();

	public:
		// Default constructor, initializes the socket as an invalid socket and doesn't increment the count
		socket_base();

	private:
		static socket_ptr create_socket( AF address_family, SOCKET_TYPE type, PROTOCOL ip_protocol );

	public:
		/*
			Creates a network socket handler, the default settings are for an IPv4 TCP socket stream, but that can be changed.
			It is important to note that when creating a `socket` object when there are no `socket` objects in existence yet, we must perform a
			call to WSAStartup, which might fail. in that event, a `std::runtime_error` will be thrown with an error message describing the issue.
		*/
		socket_base( AF address_family, SOCKET_TYPE type, PROTOCOL ip_protocol );
	
	protected:
		socket_base( socket_ptr socket, int flags = 0 );

	private:
		// Clear data in a socket without destroying the actual socket connection.
		void clear();

	public:
		// Move constructor copies the data and deletes the old object's data.
		socket_base( socket_base&& other );
		// Move assignment copies the data and deletes the old object's data.
		socket_base& operator = ( socket_base&& other );
	
		// Copy constructor deleted, makes no sense to have multiple handles for a socket.
		socket_base( const socket_base& ) = delete;
		// Copy assignment deleted, makes no sense to have multiple handles for a socket.
		socket_base& operator = ( const socket_base& ) = delete;

	private:
		static void destroy_socket( socket_ptr socket );

	public:
		// Closes a network socket handler, and if it is the last one, also shuts down WSA
		~socket_base();

	protected:
		void assert_valid() const;

	protected:
		socket_ptr pointer;
	};

	// The `network::socket` class wraps WinSock, and eventually will wrap sys/socket as an easy and portable library.
	// Though, it is not recommended to use `network::socket` by itself, but instead one should use the `network::connection` class which derives from `std::iostream` and uses `network::socket` via a `network::buffer`.
	// Also, note that `network::socket` is templated and has different definitions based on the socket types since the socket types are not interchangeable.
	// Currently the only socket type supported is `SOCKET_TYPE::STREAM`, which is a socket stream (default TCP).
	template < SOCKET_TYPE _Type >
	class basic_socket : public socket_base {};

	template <>
	class basic_socket< SOCKET_TYPE::STREAM > : public socket_base
	{
	private:
		// Protected overload that allows us to create a `network::basic_socket< SOCKET_TYPE::STREAM >` from a socket pointer type, used by `network::server_socket::accept()`
		friend class server_socket;
		basic_socket( socket_ptr socket );

		// TODO: Standard socket wrapping functions.
		// TODO: Make address (IPv4, IPv6, and full address) and port structs

	public:
		// Default constructor, creates an empty socket.
		basic_socket() = default;

		// Constructor that takes an address family and protocol to create a socket for use.
		basic_socket( AF address_family, PROTOCOL ip_protocol );

		// Move constructor copies the data and deletes the old object's data.
		basic_socket( basic_socket&& other );
		// Move assignment copies the data and deletes the old object's data.
		basic_socket& operator = ( basic_socket&& other );

		// Copy constructor is deleted, does not make sense to copy a socket.
		basic_socket( const basic_socket& other ) = delete;
		// Copy assignment is deleted, does not make sense to copy a socket.
		basic_socket& operator = ( const basic_socket& other ) = delete;
	
		// Standard socket `recv` function wrapper.
		int recieve( char *buffer, int length );
		// Standard socket `send` function wrapper.
		int send( const char *buffer, int length );
		
		// TODO: These functions need to be refined a bit, as for the usage I have thus far, they kind of need to be able to take a length of `size_t`
		// Recieves all data up to `length` into the `buffer` by repeatedly calling `socket::recieve`.
		void recieve_all( char* buffer, int length );
		// Sends all data up to `length` into the `buffer` by repeatedly calling `socket::send`.
		void send_all( const char* buffer, int length );

	public:
		void set_flags( int flags, int mask );

	protected:
		int flags;
	};

	using stream_socket = basic_socket< SOCKET_TYPE::STREAM >;

	// The `network::client_socket` class is a wrapper around the `network::socket` class, and is used to handle client connections.
	class client_socket : public stream_socket
	{
	public:
		// Default constructor, creates an empty socket.
		client_socket() = default;

		// Constructor that takes an address family and protocol to create a socket for use.
		client_socket( AF address_family, PROTOCOL ip_protocol );

		// Move constructor copies the data and deletes the old object's data.
		client_socket( client_socket&& other );
		// Move assignment copies the data and deletes the old object's data.
		client_socket& operator = ( client_socket&& other );

		// Copy constructor is deleted, does not make sense to copy a socket.
		client_socket( const client_socket& other ) = delete;
		// Copy assignment is deleted, does not make sense to copy a socket.
		client_socket& operator = ( const client_socket& other ) = delete;

		void connect( const socket_address& address );

	};

	// The `network::server_socket` class inherits from `network::socket_base` and is used to handle server operations, and returns stream sockets upon successful connection.
	class server_socket : private socket_base
	{
	public:
		// Default constructor, creates an empty socket.
		server_socket() = default;

		// Constructor that takes an address family and protocol to create a socket for use.
		server_socket( AF address_family, PROTOCOL ip_protocol );

		// Move constructor copies the data and deletes the old object's data.
		server_socket( server_socket&& other );
		// Move assignment copies the data and deletes the old object's data.
		server_socket& operator = ( server_socket&& other );

		// Copy constructor is deleted, does not make sense to copy a socket.
		server_socket( const server_socket& other ) = delete;
		// Copy assignment is deleted, does not make sense to copy a socket.
		server_socket& operator = ( const server_socket& other ) = delete;

		// Bind `network::socket` with an address.
		void bind( const socket_address& address );

		// Listen for incoming connections on the `network::socket`.
		void listen( int backlog );
		// Accept an incoming connection to the `network::socket` and get a new `network::socket` to handle that connection.
		stream_socket accept();
	};
}
// namespace network

#endif // SOCKET_HPP