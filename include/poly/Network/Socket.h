#ifndef POLY_SOCKET_H
#define POLY_SOCKET_H

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief The base class of internet sockets
/// \ingroup Network
///
///////////////////////////////////////////////////////////
class Socket
{
public:
	///////////////////////////////////////////////////////////
	/// \brief An enum for representing the status of various socket operations
	///
	///////////////////////////////////////////////////////////
	enum Status
	{
		Done,			//!< The operation finished successfully
		NotReady,		//!< The socket is not ready to send/receive data
		Partial,		//!< The socket sent part of the data (non-blocking mode)
		Disconnected,	//!< The socket is disconnected
		Error			//!< An error occured
	};

	enum
	{
		AnyPort = 0		//!< Tells sockets to use any port available
	};

public:
	///////////////////////////////////////////////////////////
	/// \brief Virtual destructor
	///
	///////////////////////////////////////////////////////////
	virtual ~Socket() { }

	///////////////////////////////////////////////////////////
	/// \brief Set whether the socket should block the thread during operations that take time
	///
	/// In blocking mode, sockets will block the thread until they have
	/// finished with their task, such as sending/receiving data.
	/// In nonblocking mode, functions will return immediately after
	/// they are called, and the status should be used to determine
	/// what happened during the call.
	///
	/// By default sockets are blocking.
	///
	/// \param blocking Determines if the socket should be set to blocking mode or not
	///
	///////////////////////////////////////////////////////////
	virtual void setBlocking(bool blocking) = 0;

	///////////////////////////////////////////////////////////
	/// \brief Check if the socket is in blocking mode
	///
	/// \return True if the socket is in blocking mode
	///
	///////////////////////////////////////////////////////////
	virtual bool isBlocking() const = 0;
};


}

#endif