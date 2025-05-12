#pragma once
#ifndef SOCKET_BUFFER_HPP
#define SOCKET_BUFFER_HPP

#include <streambuf>
#include "socket.hpp"

namespace network
{
	class socket_buffer : public std::basic_streambuf< char, std::char_traits< char > >
	{
	public:
		using char_type		= char; // Network packets are usually sent in bytes, so char will cover that nicely
		using traits_type	= ::std::char_traits< char_type >; // Using `std::char_traits< char >` for now, but might want to make our own char traits class for network packets.
		using buffer_type	= ::std::basic_streambuf< char_type, traits_type >; // Base class of streambuf.
		using int_type		= typename traits_type::int_type;
		using pos_type		= typename traits_type::pos_type;
		using off_type		= typename traits_type::off_type;
		using streamsize	= ::std::streamsize;
		
	public:
		// TODO: Create a constructor that can build the stream type socket.

		// Constructor that takes a maximum packet size, default is 1024 bytes, actual buffer size will be 2x the max packet size, to allow multiple packets to sit in the buffer.
		socket_buffer( streamsize max_packet_size = 1024 );
		~socket_buffer();
		
	private:
		// TODO: Define the functions below.
		// I will need to read over the `streambuf` implementation information to see what we need to do here.
		
		// TODO: Write fuction comments for each function, might have the AI do it for me.
		void imbue( const std::locale& new_locale ) override;
		buffer_type* setbuf( char_type* buffer, streamsize size ) override; // This function might not be used if the class itself handles the lifetime of it's own buffer.
		std::streampos seekoff( off_type offset, std::ios_base::seekdir direction, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out ) override;
		std::streampos seekpos( std::streampos position, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out ) override;
		int sync() override;
		streamsize showmanyc() override;
		int_type underflow() override;
		int_type uflow() override;
		int_type overflow( int_type c = traits_type::eof() ) override;
		int_type pbackfail( int_type c = traits_type::eof() ) override;
		streamsize xsgetn( char_type* buffer, streamsize count ) override;
		streamsize xsputn( const char_type* buffer, streamsize count ) override;
		
	private:
		// Socket data member, which will be used to send and receive data.
		basic_socket< SOCKET_TYPE::STREAM >* connection;

		// Maximum packet size, default is 1024 bytes.
		const streamsize max_packet_size;
	};
	
}
// namespace network
	
#endif // SOCKET_BUFFER_HPP