#include "codex.hpp"

#include <cstring>

namespace network
{
	void codex::copy_data( const char* input, char* output )
	{ std::memcpy( output, input, std::strlen( input ) ); }

	void codex::do_encrypt( const char* input, char* output )
	{ copy_data( input, output ); }

	void codex::do_decrypt( const char* input, char* output )
	{ copy_data( input, output ); }

	void codex::encrypt( const char* input, char* output )
	{ do_encrypt( input, output ); }

	void codex::decrypt( const char* input, char* output )
	{ do_decrypt( input, output ); }

	basic_codex::basic_codex( char offset ) : offset( offset % max_offset ) {} // Restrict offset to a byte

	char basic_codex::do_shift( const char& input, char shift )
	{ return input << shift | input >> ( max_offset - shift ); }

	void basic_codex::do_encrypt( const char* input, char* output )
	{
		for ( size_t index = 0; index < std::strlen( input ); ++index ) 
			output[ index ] = do_shift( input[ index ], this->offset );
	}

	void basic_codex::do_decrypt( const char* input, char* output )
	{
		for ( size_t index = 0; index < std::strlen( input ); ++index ) 
			output[ index ] = do_shift( input[ index ], ( max_offset - this->offset % 8 ) );
	}

	recursion_codex::recursion_codex( char initial_modifier ) : modifier( initial_modifier ) {}

	char recursion_codex::update_modifier( char next_modifier )
	{
		char prev_modifier = this->modifier;
		this->modifier = next_modifier;
		return prev_modifier;
	}

	void recursion_codex::do_encrypt( const char* input, char* output )
	{
		for ( size_t index = 0; index < std::strlen( input ); ++index )
			this->update_modifier( output[ index ] = input[ index ] ^ this->modifier );
	}

	void recursion_codex::do_decrypt( const char* input, char* output )
	{
		for ( size_t index = 0; index < std::strlen( input ); ++index )
			output[ index ] = input[ index ] ^ this->update_modifier( input[ index ] );
	}
}
// namespace network
