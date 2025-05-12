#pragma once
#ifndef NETWORK_CONNECTION_HPP
#define NETWORK_CONNECTION_HPP

#include "network_buffer.hpp"

#include <ostream>
#include <istream>
#include <cstring>

namespace network
{
	// A base class for encryption and decryption functions to be used in the `connection` class.
	// This class is meant to be inherited by the user to implement their own encryption and decryption functions.
	class codex
	{
	protected:
		// Virtual encryption function to be overriden by the user's specific implementation.
		// The base implementation is just to return the data as is.
		virtual void do_encrypt( const char* input, char* output );

		// Virtual decryption function to be overriden by the user's specific implementation.
		// The base implementation is just to return the data as is.
		virtual void do_decrypt( const char* input, char* output );
	
	public:
		// Public encryption function that just calls the protected `do_encrypt` function, which should be overridden by a derived class.
		void encrypt( const char* input, char* output )
		{ do_encrypt( input, output ); }
		void decrypt( const char* input, char* output )
		{ do_decrypt( input, output ); }
	};

	class null_codex : public codex
	{
	protected:
		// A helper function to copy data from the input buffer to the output buffer.
		void copy_data( const char* input, char* output )
		{ std::memcpy( output, input, std::strlen( input ) ); }

		// An empty implementation of the encryption function, which does nothing, marked as final so this class cannot be inherited.
		void do_encrypt( const char* input, char* output ) override final
		{ copy_data( input, output ); }

		// An empty implementation of the decryption function, which does nothing, marked as final so this class cannot be inherited.
		void do_decrypt( const char* input, char* output ) override final
		{ copy_data( input, output ); }
	};
	
	class connection : public std::iostream
	{
	public:
		using char_type		= char;
		using traits_type	= std::char_traits< char_type >;
		using stream_type	= std::basic_iostream< char_type, traits_type >;
		using int_type		= typename traits_type::int_type;
		using pos_type		= typename traits_type::pos_type;
		using off_type		= typename traits_type::off_type;
		using streamsize	= std::streamsize;

		// Using the `network::socket_buffer` class to handle buffering data and performing send/recieve operations.
		using buffer_type	= socket_buffer;

		connection();
		~connection();

		// Sets the encryption and decryption functions to be used for encrypting and decrypting data.
		codex set_codex( codex encryption );

		/*
		 - All operator << functions defined below are blocking operations.
		 - They will wait until the data is available to be sent or received.
		 -
		 - All operator >> functions defined below will pass data to the buffer, without sending.
		 - To send the data, you must use `std::flush` or call the `flush()` function on the `connection` object.
		 -
		 - A call to the operator << function will flush the buffer before requesting new data.
		 */
		
		// Pass a string to be sent over the connection.
		connection& operator << ( const std::string& data );

		// Retrieve a string from the connection.
		connection& operator >> ( std::string& data );

		// Pass a general datatype over the connection.
		template < class _Data >
		connection& operator << ( const _Data& data )
		{
			const char* send_buffer = reinterpret_cast< const char* >( &data );
			this->write( this->codex.encrypt( send_buffer ), sizeof( data ) );
			return *this;
		}
		template < class _Data >
		connection& operator >> ( _Data& data )
		{
			char* recv_buffer = reinterpret_cast< char* >( &data );
			return *this;
		}

		
		// public:
		// // The following are stream operators to pull and push data through the network, using unions.
		// // Those unions are expected to have a character array by the name of `raw` which is the same size as the struct storing the ordered data.

		// template < class _Data >
		// socket& operator << ( const _Data& data )
		// {
		// 	send_all( data.raw, sizeof( data.raw ) );
		// 	return *this;
		// }
		// template < class _Data >
		// socket& operator << ( _Data&& data )
		// {
		// 	send_all( data.raw, sizeof( data.raw ) );
		// 	return *this;
		// }
		// template < class _Data >
		// socket& operator >> ( _Data& data )
		// {
		// 	recieve_all( data.raw, sizeof( data.raw ) );
		// 	return *this;
		// }
		
		// // The following are stream operators to pull and push data through the network on standard strings, resizing as needed.

		// template < class _Char >
		// socket& operator << ( const std::basic_string< _Char >& data )
		// {
		// 	send_length( data.length() );
		// 	send_all( data.c_str(), data.length() );
		// 	return *this;
		// }
		// template < class _Char >
		// socket& operator << ( std::basic_string< _Char >&& data )
		// {
		// 	send_length( data.length() );
		// 	send_all( data.c_str(), data.length() );
		// 	return *this;
		// }
		// template < class _Char >
		// socket& operator >> ( std::basic_string< _Char >& data )
		// {
		// 	size_t incoming = recieve_length();
		// 	data.resize( incoming );
		// 	recieve_all( data.data(), incoming );
		// 	return *this;
		// }
	

	private:
		// The stream buffer that will be used to send and receive data across the network.
		buffer_type my_buffer;

		// The codex containing the encryption and decryption function pair, default codex does no encryption or decryption.
		codex my_encryption;

		// Tracks whether the connection is threaded or not, defaults to true.
		bool is_threaded = true;
	};
	
}
// namespace network

#endif // NETWORK_CONNECTION_HPP