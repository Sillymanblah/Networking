#include "network_buffer.hpp"

namespace network
{
	socket_buffer::socket_buffer( streamsize max_packet_size ) : buffer_type(), connection(), max_packet_size( max_packet_size )
	{
		if ( max_packet_size < 1 ) throw std::invalid_argument( "Maximum packet size must be greater than 0." );
		// TODO: if max_packet_size is less than 100, we should log a warning.
		if ( max_packet_size > 1024 ) throw std::invalid_argument( "Maximum packet size must be less than or equal to 1024." );

		const streamsize buffer_size = max_packet_size * 2; // Set the buffer size to twice the maximum packet size.
		char_type* buffer = new char_type[ buffer_size ]; // Allocate a buffer of 1024 bytes.

		setg( buffer, buffer, buffer + buffer_size ); // Set the get area to null pointers.
		setp( buffer, buffer + buffer_size ); // Set the put area to null pointers.
	}

	// Deletes the socket_bufferfer object
	socket_buffer::~socket_buffer()
	{ delete [] pbase(); }
	
	// I will need to read over the `socket_buffer` implementation information to see what we need to do here.
	void socket_buffer::imbue( const std::locale& new_locale )
	{

	}

	std::streampos socket_buffer::seekoff( off_type offset, std::ios_base::seekdir direction, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out )
	{

	}

	std::streampos socket_buffer::seekpos( std::streampos position, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out )
	{

	}

	int socket_buffer::sync()
	{

	}

	socket_buffer::streamsize socket_buffer::showmanyc()
	{

	}

	socket_buffer::int_type socket_buffer::underflow()
	{

	}

	socket_buffer::int_type socket_buffer::uflow()
	{

	}

	socket_buffer::int_type socket_buffer::overflow( int_type c = traits_type::eof() )
	{

	}

	socket_buffer::int_type socket_buffer::pbackfail( int_type c = traits_type::eof() )
	{

	}

	socket_buffer::streamsize socket_buffer::xsgetn( char_type* buffer, streamsize count )
	{

	}

	socket_buffer::streamsize socket_buffer::xsputn( const char_type* buffer, streamsize count )
	{

	}
}
// namespace network
