#pragma once
#ifndef SOCKET_ERROR_HPP
#define SOCKET_ERROR_HPP

#include <exception>

namespace network
{
	class socket_error : private std::exception
	{
	public:
		// Our uniquely defined error codes, that can also be sent into the socket_error constructor.
		enum codes : int
		{
			BAD_ADDRESS_TYPE	= 1,
			UNIMPLEMENTED		= 2,
			ADDRESS_NOT_FOUND	= 3,
			SOCKET_NOT_VALID	= 4,
			OPERATION_FAILED	= 5,
			OPERATION_TIMEOUT	= 6,
		};

	private:
		static const char* get_error_message( int code ) noexcept;

	public:
		socket_error( int error_code ) noexcept;
		socket_error( const socket_error& ) noexcept = default;
		socket_error& operator = ( const socket_error& ) noexcept = default;
		socket_error( socket_error&& ) noexcept = default;
		socket_error& operator = ( socket_error&& ) noexcept = default;

		const char* what() const noexcept override;

	private:
		int code;
	};
}
// namespace network


#endif // SOCKET_ERROR_HPP