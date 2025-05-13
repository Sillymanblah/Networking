#pragma once
#ifndef CODEX_HPP
#define CODEX_HPP

namespace network
{
	// A base class for encryption and decryption functions to be used in the `connection` class.
	// This class is meant to be inherited by the user to implement their own encryption and decryption functions.
	class codex
	{
	private:
		// A helper function to copy data from the input buffer to the output buffer.
		void copy_data( const char* input, char* output );

	protected:
		// Virtual encryption function to be overriden by the user's specific implementation.
		// The base implementation is just to return the data as is.
		virtual void do_encrypt( const char* input, char* output );

		// Virtual decryption function to be overriden by the user's specific implementation.
		// The base implementation is just to return the data as is.
		virtual void do_decrypt( const char* input, char* output );
	
	public:
		// Public encryption function that just calls the protected `do_encrypt` function, which should be overridden by a derived class.
		void encrypt( const char* input, char* output );

		// Public decryption function that just calls the protected `do_decrypt` function, which should be overridden by a derived class.
		void decrypt( const char* input, char* output );
	};
}
// namespace network

#endif // CODEX_HPP