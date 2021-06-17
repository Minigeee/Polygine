#ifndef POLY_TCP_SOCKET_H
#define POLY_TCP_SOCKET_H

#include <poly/Core/Time.h>

#include <poly/Network/IpAddress.h>
#include <poly/Network/Packet.h>
#include <poly/Network/Socket.h>

#include <SFML/Network/TcpSocket.hpp>

namespace poly
{


class TcpListener;


///////////////////////////////////////////////////////////
/// \brief A socket used to transfer data through the internet using the TCP protocol
///
///////////////////////////////////////////////////////////
class TcpSocket : public Socket
{
	friend TcpListener;

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	TcpSocket();

	///////////////////////////////////////////////////////////
	/// \brief Attempt to connect the socket to a remote peer
	///
	/// A connection is attempted, and the thread is blocked until
	/// the task is completed (in blocking mode). The function will
	/// automatically return if the connection isn't made within the
	/// timeout value. A custom timeout value can be set, but its default
	/// value is zero, which indicates that the system default should
	/// be used.
	///
	/// \param address The address of the remote peer to connect to
	/// \param port The remote port to make a connection to
	/// \param timeout The amount of time to wait before cancelling the task
	///
	/// \return The status of the task (will equal Status::Done if successful)
	///
	///////////////////////////////////////////////////////////
	Status connect(const IpAddress& address, Uint16 port, Time timeout = Time());

	///////////////////////////////////////////////////////////
	/// \brief Disconnect from the remote connection, if the connection exists
	///
	///////////////////////////////////////////////////////////
	void disconnect();

	///////////////////////////////////////////////////////////
	/// \brief Send data to the remote peer
	///
	/// This sends raw data to the remote peer, without handling
	/// data type sizes and endianness. The thread is blocked until
	/// all data is sent in blocking mode, and in non-blocking mode,
	/// data is sent until the function is returned. Because of this
	/// it is possible for only a portion of the data to be sent in
	/// non-blocking mode. To handle partial data send, use
	/// send(const void*,Uint32,Uint32&) to track how many bytes
	/// were sent.
	///
	/// \param data A pointer to the data to send
	/// \param size The number of bytes to send
	///
	/// \return The status of the send task (will equal Status::Done if successful)
	///
	///////////////////////////////////////////////////////////
	Status send(const void* data, Uint32 size);

	///////////////////////////////////////////////////////////
	/// \brief Send data to the remote peer
	///
	/// This sends raw data to the remote peer, without handling
	/// data type sizes and endianness. The thread is blocked until
	/// all data is sent in blocking mode, and in non-blocking mode,
	/// data is sent until the function is returned. Because of this
	/// it is possible for only a portion of the data to be sent in
	/// non-blocking mode. The actual number of bytes that was sent
	/// will be written to \a sent.
	///
	/// \param data A pointer to the data to send
	/// \param size The number of bytes to send
	/// \param sent The reference variable that will contain the number of bytes that was actually sent
	///
	/// \return The status of the send task (will equal Status::Done if successful)
	///
	///////////////////////////////////////////////////////////
	Status send(const void* data, Uint32 size, Uint32& sent);

	///////////////////////////////////////////////////////////
	/// \brief Send data to the remote peer
	///
	/// This sends data using a formatted packet. Packets are used
	/// to handle data type sizes and endianness when transferring
	/// data over the internet, as these properties may not be the
	/// same for all machines.
	///
	/// In non-blocking mode, if the return status is Status::Partial,
	/// the same packet must be resent before sending anything else
	/// to ensure the data is uncorrupted.
	///
	/// \param packet The packet of data to send
	///
	/// \return The status of the send task (will equal Status::Done if successful)
	///
	///////////////////////////////////////////////////////////
	Status send(Packet& packet);

	///////////////////////////////////////////////////////////
	/// \brief Receive data from the remote peer
	///
	/// This receives raw data from the connection, and if no data
	/// is ready to be received, the thread will be blocked by this
	/// function until data is ready to be received.
	///
	/// \param data A pointer to the buffer to write the data into
	/// \param max The maximum number of bytes to receive (to prevent writing too much data)
	/// \param received The integer where the actual number of bytes that was received is written
	///
	/// \return The status of the receive task (Status::Done if successful, or Status::NotReady if no data is present in non-blocking mode)
	///
	///////////////////////////////////////////////////////////
	Status receive(void* data, Uint32 max, Uint32& received);

	///////////////////////////////////////////////////////////
	/// \brief Receive data from the remote peer
	///
	/// This receives formatted packet data from the connection, and if no data
	/// is ready to be received, the thread will be blocked by this
	/// function until data is ready to be received.
	///
	/// \param packet A refernce to the output packet object where the received data will be written
	///
	/// \return The status of the receive task (Status::Done if successful, or Status::NotReady if no data is present in non-blocking mode)
	///
	///////////////////////////////////////////////////////////
	Status receive(Packet& packet);

	///////////////////////////////////////////////////////////
	/// \copydoc Socket::setBlocking
	///
	///////////////////////////////////////////////////////////
	void setBlocking(bool blocking) override;

	///////////////////////////////////////////////////////////
	/// \brief Get the local port the socket is bound to
	///
	/// \return The local port the socket is bound to
	///
	///////////////////////////////////////////////////////////
	Uint16 getLocalPort() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the ip address of the remote peer
	///
	/// \return The ip address of the remote peer
	///
	///////////////////////////////////////////////////////////
	IpAddress getRemoteAddress() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the port the remote peer is bound to
	///
	/// \return The port the remote peer is bound to
	///
	///////////////////////////////////////////////////////////
	Uint16 getRemotePort() const;

	///////////////////////////////////////////////////////////
	/// \copydoc Socket::isBlocking
	///
	///////////////////////////////////////////////////////////
	bool isBlocking() const override;

private:
	sf::TcpSocket m_socket;			//!< The internal SFML tcp socket
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::TcpSocket
/// \ingroup Network
///
/// This socket type uses the TCP protocol to communicate data over
/// the internet. The TCP protocol requires a connection with its
/// peer to communicate, meaning that it can only communicate to
/// a single peer through a socket. TCP is more reliable than UDP,
/// but is slightly slower because of the overhead. It guarantees that
/// any data that is sent will be correct, meaning that data will
/// arrive in the correct order, no data will be missing or duplicated,
/// and none of the data will be corrupt.
///
/// To use a TCP socket, first connect to a remote peer by providing
/// the remote ip address and port to connect to by calling the connect()
/// function. If connection is successful, the socket can communicate
/// using send() and receive(). Data can be sent in its raw format
/// as a byte stream, or as formatted packages using Packet.
/// A Packet hides away some of the lower level details, such
/// as handling endianness, and provides a slightly nicer interface
/// for sending data. Check the Packet documentation for more details.
///
/// Sockets are automatically disconnected when they are destroyed.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// TcpSocket socket;
/// if (socket.connect("localhost", 50000) != Status::Done)
///		// Failed to connect...
///
/// // Send data using a packet
/// Packet packet;
/// packet << 50 << "Hello World!";
/// socket.send(packet);
///
/// \endcode
///
///////////////////////////////////////////////////////////