#ifndef POLY_TCP_LISTENER_H
#define POLY_TCP_LISTENER_H

#include <poly/Network/IpAddress.h>
#include <poly/Network/Socket.h>
#include <poly/Network/TcpSocket.h>

#include <SFML/Network/TcpListener.hpp>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief A TCP socket specialized for listening for and accepting connections
///
///////////////////////////////////////////////////////////
class TcpListener : public Socket
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	TcpListener();

	///////////////////////////////////////////////////////////
	/// \brief Start listening for incoming TCP connection requests
	///
	/// The listener will start listening on the specified port
	/// for incoming connection requests. If the listener was already
	/// listening on another port, it will close the other port and
	/// listen on this one.
	///
	/// \see accept
	///
	/// \param port The port to listen for connections on
	/// \param address The address of the interface to listen on
	///
	/// \return The status of the function (will equal Status::Done if successful)
	///
	///////////////////////////////////////////////////////////
	Status listen(Uint16 port, const IpAddress& address = IpAddress::Any);

	///////////////////////////////////////////////////////////
	/// \brief Stop listening for incoming connection requests
	///
	///////////////////////////////////////////////////////////
	void close();

	///////////////////////////////////////////////////////////
	/// \brief Accept a connection request and create a socket containing the connection info
	///
	/// In blocking mode, this function will not return until a connection
	/// has been accepted.
	///
	/// \param socket The socket that the new connection info will be put in
	///
	/// \return The status of the task (will equal Status::Done if successful)
	///
	///////////////////////////////////////////////////////////
	Status accept(TcpSocket& socket);

	///////////////////////////////////////////////////////////
	/// \copydoc Socket::setBlocking
	///
	///////////////////////////////////////////////////////////
	void setBlocking(bool blocking) override;

	///////////////////////////////////////////////////////////
	/// \brief Get the local port the listener is bound to
	///
	/// \return The local port the listener is bound to
	///
	///////////////////////////////////////////////////////////
	Uint16 getLocalPort() const;

	///////////////////////////////////////////////////////////
	/// \copydoc Socket::isBlocking
	///
	///////////////////////////////////////////////////////////
	bool isBlocking() const override;

private:
	sf::TcpListener m_listener;			//!< The internal SFML listener
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::TcpListener
/// \ingroup Network
///
/// The listener binds itself to a port and waits for incoming
/// connection requests made to that port. The listener can then
/// accept any incoming connections, and a new socket is made for
/// each connection that is accepted.
///
/// To use the listener, call listen() with the desired port number
/// to bind to. Then call accept() to wait for a connection and accept
/// it (in blocking mode) to create a new socket for the connection.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Start listening on a port
/// TcpListener listener;
/// listener.listen(50000);
///
/// // Loop to accept all connections
/// while (true)
/// {
///		TcpSocket socket;
///		// The thread is blocked right here until a new connection is requested (in blocking mode)
///		if (listener.accept(socket) == Socket::Done)
///		{
///			// New connection, that can be communicated with through socket
///		}
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////