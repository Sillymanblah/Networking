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

	// The below are some examples of how to use the codex, note that when building a codex, it is good practice to not have the codex publicly published for security reasons.

	// Basic codex simply uses a bitwise algorithm
	class basic_codex : public codex
	{
	public:
		basic_codex( char offset );
		static constexpr char max_offset = 8;

	private:
		static char do_shift( const char& input, char offset );

	protected:
		void do_encrypt( const char* input, char* output ) override;
		void do_decrypt( const char* input, char* output ) override;
	
	private:
		char offset;
	};

	// Recursion codex uses the previous set of data to xor the next set of data, which allows the server and client to vary throughout the communication cycle.
	class recursion_codex : public codex
	{
	public:
		// Constructor which can set the initial modifier value.
		recursion_codex( char initial_modifier = 0 );

	private:
		// Updates the modifier and returns the previous one.
		char update_modifier( char next_modifier );

	protected:
		// Uses modifier to update a character, then sets the modifier based on the result of that operation.
		void do_encrypt( const char* input, char* output ) override;
		// Uses modifier to update a character and then sets the modifier to the previously modified character.
		void do_decrypt( const char* input, char* output ) override;

	private:
		char modifier;
	};
}
// namespace network

#endif // CODEX_HPP