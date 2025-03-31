#include "socket_error.hpp"

namespace network
{
	const char* socket_error::get_error_message( int error_code ) noexcept
	{
		switch ( error_code )
		{
		case 10004L: // WSAEINTR
			return "A blocking operation was interrupted by a call to WSACancelBlockingCall.";
		case 10009L: // WSAEBADF
			return "The file handle supplied is not valid.";
		case 10013L: // WSAEACCES
			return "An attempt was made to access a socket in a way forbidden by its access permissions.";
		case 10014L: // WSAEFAULT
			return "The system detected an invalid pointer address in attempting to use a pointer argument in a call.";
		case 10022L: // WSAEINVAL
			return "An invalid argument was supplied.";
		case 10024L: // WSAEMFILE
			return "Too many open sockets.";
		case 10035L: // WSAEWOULDBLOCK
			return "A non-blocking socket operation could not be completed immediately.";
		case 10036L: // WSAEINPROGRESS
			return "A blocking operation is currently executing.";
		case 10037L: // WSAEALREADY
			return "An operation was attempted on a non-blocking socket that already had an operation in progress.";
		case 10038L: // WSAENOTSOCK
			return "An operation was attempted on something that is not a socket.";
		case 10039L: // WSAEDESTADDRREQ
			return "A required address was omitted from an operation on a socket.";
		case 10040L: // WSAEMSGSIZE
			return "A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram into was smaller than the datagram itself.";
		case 10041L: // WSAEPROTOTYPE
			return "A protocol was specified in the socket function call that does not support the semantics of the socket type requested.";
		case 10042L: // WSAENOPROTOOPT
			return "An unknown, invalid, or unsupported option or level was specified in a getsockopt or setsockopt call.";
		case 10043L: // WSAEPROTONOSUPPORT
			return "The requested protocol has not been configured into the system, or no implementation for it exists.";
		case 10044L: // WSAESOCKTNOSUPPORT
			return "The support for the specified socket type does not exist in this address family.";
		case 10045L: // WSAEOPNOTSUPP
			return "The attempted operation is not supported for the type of object referenced.";
		case 10046L: // WSAEPFNOSUPPORT
			return "The protocol family has not been configured into the system or no implementation for it exists.";
		case 10047L: // WSAEAFNOSUPPORT
			return "An address incompatible with the requested protocol was used.";
		case 10048L: // WSAEADDRINUSE
			return "Only one usage of each socket address (protocol/network address/port) is normally permitted.";
		case 10049L: // WSAEADDRNOTAVAIL
			return "The requested address is not valid in its context.";
		case 10050L: // WSAENETDOWN
			return "A socket operation encountered a dead network.";
		case 10051L: // WSAENETUNREACH
			return "A socket operation was attempted to an unreachable network.";
		case 10052L: // WSAENETRESET
			return "The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress.";
		case 10053L: // WSAECONNABORTED
			return "An established connection was aborted by the software in your host machine.";
		case 10054L: // WSAECONNRESET
			return "An existing connection was forcibly closed by the remote host.";
		case 10055L: // WSAENOBUFS
			return "An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.";
		case 10056L: // WSAEISCONN
			return "A connect request was made on an already connected socket.";
		case 10057L: // WSAENOTCONN
			return "A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied.";
		case 10058L: // WSAESHUTDOWN
			return "A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call.";
		case 10059L: // WSAETOOMANYREFS
			return "Too many references to some kernel object.";
		case 10060L: // WSAETIMEDOUT
			return "A connection attempt failed because the connected party did not properly respond after a period of time, or established connection failed because connected host has failed to respond.";
		case 10061L: // WSAECONNREFUSED
			return "No connection could be made because the target machine actively refused it.";
		case 10062L: // WSAELOOP
			return "Cannot translate name.";
		case 10063L: // WSAENAMETOOLONG
			return "Name component or name was too long.";
		case 10064L: // WSAEHOSTDOWN
			return "A socket operation failed because the destination host was down.";
		case 10065L: // WSAEHOSTUNREACH
			return "A socket operation was attempted to an unreachable host.";
		case 10066L: // WSAENOTEMPTY
			return "Cannot remove a directory that is not empty.";
		case 10067L: // WSAEPROCLIM
			return "A Windows Sockets implementation may have a limit on the number of applications that may use it simultaneously.";
		case 10068L: // WSAEUSERS
			return "Ran out of quota.";
		case 10069L: // WSAEDQUOT
			return "Ran out of disk quota.";
		case 10070L: // WSAESTALE
			return "File handle reference is no longer available.";
		case 10071L: // WSAEREMOTE
			return "Item is not available locally.";
		case 10091L: // WSASYSNOTREADY
			return "WSAStartup cannot function at this time because the underlying system it uses to provide network services is currently unavailable.";
		case 10092L: // WSAVERNOTSUPPORTED
			return "The Windows Sockets version requested is not supported.";
		case 10093L: // WSANOTINITIALISED
			return "Either the application has not called WSAStartup, or WSAStartup failed.";
		case 10101L: // WSAEDISCON
			return "Returned by WSARecv or WSARecvFrom to indicate the remote party has initiated a graceful shutdown sequence.";
		case 10102L: // WSAENOMORE
			return "No more results can be returned by WSALookupServiceNext.";
		case 10103L: // WSAECANCELLED
			return "A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled.";
		case 10104L: // WSAEINVALIDPROCTABLE
			return "The procedure call table is invalid.";
		case 10105L: // WSAEINVALIDPROVIDER
			return "The requested service provider is invalid.";
		case 10106L: // WSAEPROVIDERFAILEDINIT
			return "The requested service provider could not be loaded or initialized.";
		case 10107L: // WSASYSCALLFAILURE
			return "A system call has failed.";
		case 10108L: // WSASERVICE_NOT_FOUND
			return "No such service is known. The service cannot be found in the specified name space.";
		case 10109L: // WSATYPE_NOT_FOUND
			return "The specified class was not found.";
		case 10110L: // WSA_E_NO_MORE
			return "No more results can be returned by WSALookupServiceNext.";
		case 10111L: // WSA_E_CANCELLED
			return "A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled.";
		case 10112L: // WSAEREFUSED
			return "A database query failed because it was actively refused.";
		case 11001L: // WSAHOST_NOT_FOUND
			return "No such host is known.";
		case 11002L: // WSATRY_AGAIN
			return "This is usually a temporary error during hostname resolution and means that the local server did not receive a response from an authoritative server.";
		case 11003L: // WSANO_RECOVERY
			return "A non-recoverable error occurred during a database lookup.";
		case 11004L: // WSANO_DATA
			return "The requested name is valid, but no data of the requested type was found.";
		case 11005L: // WSA_QOS_RECEIVERS
			return "At least one reserve has arrived.";
		case 11006L: // WSA_QOS_SENDERS
			return "At least one path has arrived.";
		case 11007L: // WSA_QOS_NO_SENDERS
			return "There are no senders.";
		case 11008L: // WSA_QOS_NO_RECEIVERS
			return "There are no receivers.";
		case 11009L: // WSA_QOS_REQUEST_CONFIRMED
			return "Reserve has been confirmed.";
		case 11010L: // WSA_QOS_ADMISSION_FAILURE
			return "Error due to lack of resources.";
		case 11011L: // WSA_QOS_POLICY_FAILURE
			return "Rejected for administrative reasons - bad credentials.";
		case 11012L: // WSA_QOS_BAD_STYLE
			return "Unknown or conflicting style.";
		case 11013L: // WSA_QOS_BAD_OBJECT
			return "Problem with some part of the filterspec or providerspecific buffer in general.";
		case 11014L: // WSA_QOS_TRAFFIC_CTRL_ERROR
			return "Problem with some part of the flowspec.";
		case 11015L: // WSA_QOS_GENERIC_ERROR
			return "General QOS error.";
		case 11016L: // WSA_QOS_ESERVICETYPE
			return "An invalid or unrecognized service type was found in the flowspec.";
		case 11017L: // WSA_QOS_EFLOWSPEC
			return "An invalid or inconsistent flowspec was found in the QOS structure.";
		case 11018L: // WSA_QOS_EPROVSPECBUF
			return "Invalid QOS provider-specific buffer.";
		case 11019L: // WSA_QOS_EFILTERSTYLE
			return "An invalid QOS filter style was used.";
		case 11020L: // WSA_QOS_EFILTERTYPE
			return "An invalid QOS filter type was used.";
		case 11021L: // WSA_QOS_EFILTERCOUNT
			return "An incorrect number of QOS FILTERSPECs were specified in the FLOWDESCRIPTOR.";
		case 11022L: // WSA_QOS_EOBJLENGTH
			return "An object with an invalid ObjectLength field was specified in the QOS provider-specific buffer.";
		case 11023L: // WSA_QOS_EFLOWCOUNT
			return "An incorrect number of flow descriptors was specified in the QOS structure.";
		case 11024L: // WSA_QOS_EUNKOWNPSOBJ
			return "An unrecognized object was found in the QOS provider-specific buffer.";
		case 11025L: // WSA_QOS_EPOLICYOBJ
			return "An invalid policy object was found in the QOS provider-specific buffer.";
		case 11026L: // WSA_QOS_EFLOWDESC
			return "An invalid QOS flow descriptor was found in the flow descriptor list.";
		case 11027L: // WSA_QOS_EPSFLOWSPEC
			return "An invalid or inconsistent flowspec was found in the QOS provider specific buffer.";
		case 11028L: // WSA_QOS_EPSFILTERSPEC
			return "An invalid FILTERSPEC was found in the QOS provider-specific buffer.";
		case 11029L: // WSA_QOS_ESDMODEOBJ
			return "An invalid shape discard mode object was found in the QOS provider specific buffer.";
		case 11030L: // WSA_QOS_ESHAPERATEOBJ
			return "An invalid shaping rate object was found in the QOS provider-specific buffer.";
		case 11031L: // WSA_QOS_RESERVED_PETYPE
			return "A reserved policy element was found in the QOS provider-specific buffer.";
		case 11032L: // WSA_SECURE_HOST_NOT_FOUND
			return "No such host is known securely.";
		case 11033L: // WSA_IPSEC_NAME_POLICY_ERROR
			return "Name based IPSEC policy could not be added.";
		default:
			return "Unknown socket error code!";
		}
	}
	
	socket_error::socket_error( int error_code ) noexcept : code( error_code ) {}
	const char* socket_error::what() const noexcept
	{ return get_error_message( code ); }
}
// namespace network