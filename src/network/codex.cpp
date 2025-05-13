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
}
// namespace network
