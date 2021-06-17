#ifndef POLY_UDP_SOCKET_H
#define POLY_UDP_SOCKET_H

#include <poly/Core/DataTypes.h>

#include <poly/Network/IpAddress.h>
#include <poly/Network/Packet.h>
#include <poly/Network/Socket.h>

#include <SFML/Network/UdpSocket.hpp>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief A socket used to transfer data through the internet using the UDP protocol
///
///////////////////////////////////////////////////////////
class UdpSocket : public Socket
{
public:
	enum
	{
		MaxDatagramSize = sf::UdpSocket::MaxDatagramSize	//!< The maximum amount of data that can be sent in a single UDP datagram
	};

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	UdpSocket();

	///////////////////////////////////////////////////////////
	/// \brief Bind the UDP socket to a port
	///
	/// The UDP socket must be bound to a port to receive data.
	/// A port number may be assigned, or the value Socket::AnyPort
	/// can be used to automatically choose an available port number.
	/// If the socket is already bound to a port, it will be unbound
	/// then rebound to the specified port.
	///
	/// \param port The port number to bind to
	/// \param address The address of the interface to bind to
	///
	///////////////////////////////////////////////////////////
	Status bind(Uint16 port, const IpAddress& address = IpAddress::Any);

	///////////////////////////////////////////////////////////
	/// \brief Unbind the socket from the port it is currently bound to
	///
	/// Nothing happens if the socket is not bound.
	///
	///////////////////////////////////////////////////////////
	void unbind();

	///////////////////////////////////////////////////////////
	/// \brief Send data to a remote peer
	///
	/// This sends raw data to a remote host, without handling
	/// data type sizes and endianness. The thread is blocked until
	/// all data is sent in blocking mode.
	///
	/// Because UDP is a connectionless protocol, the ip address and
	/// port of the remote peer must be specified.
	///
	/// \param data A pointer to the data to send
	/// \param size The number of bytes to send
	/// \param address The ip address of the remote peer to send data to
	/// \param port The port number of the remote peer to send data to
	///
	/// \return The status of the send task (will equal Status::Done if successful)
	///
	///////////////////////////////////////////////////////////
	Status send(const void* data, Uint32 size, const IpAddress& address, Uint16 port);

	///////////////////////////////////////////////////////////
	/// \brief Send data to a remote peer
	///
	/// This sends data using a formatted packet. Packets are used
	/// to handle data type sizes and endianness when transferring
	/// data over the internet, as these properties may not be the
	/// same for all machines. This function blocks the thread until
	/// all data has been sent when the socket is in blocking mode.
	///
	/// Because UDP is a connectionless protocol, the ip address and
	/// port of the remote peer must be specified.
	///
	/// \param packet The packet of data to send
	/// \param address The ip address of the remote peer to send data to
	/// \param port The port number of the remote peer to send data to
	///
	/// \return The status of the send task (will equal Status::Done if successful)
	///
	///////////////////////////////////////////////////////////
	Status send(Packet& packet, const IpAddress& address, Uint16 port);

	///////////////////////////////////////////////////////////
	/// \brief Receive data from a remote peer
	///
	/// This receives data from a remote peer as raw data. If no
	/// data is ready to be received, the thread will be blocked if
	/// the socket is currently in blocking mode. If it is in non-blocking
	/// mode, the return status will be Status::NotReady if no data is
	/// available.
	///
	/// Because UDP is a connectionless protocol, the remote address
	/// and port will be recorded in the \a address and \a port
	/// parameters. The received data will be written into the location
	/// at the \a data pointer and the amount of data that is written
	/// will be capped by the \a max parameter. The actual number of
	/// bytes that was written will be put into the \a received
	/// parameter.
	///
	/// \param data The pointer to the location where data will be copied into
	/// \param max The maximum number of bytes to receive (so that not too much data is written into the buffer)
	/// \param received The reference variable where the actual number of bytes that was received will be placed into
	/// \param address The reference variable the remote address will be placed into
	/// \param port The reference variable where the remote port will be placed into
	///
	/// \return The status of the receive task (Status::Done if successful, or Status::NotReady if no data is present in non-blocking mode)
	///
	///////////////////////////////////////////////////////////
	Status receive(void* data, Uint32 max, Uint32& received, IpAddress& address, Uint16& port);

	///////////////////////////////////////////////////////////
	/// \brief Receive data from a remote peer
	///
	/// This receives data from a remote peer as a formated packet. If no
	/// data is ready to be received, the thread will be blocked if
	/// the socket is currently in blocking mode. If it is in non-blocking
	/// mode, the return status will be Status::NotReady if no data is
	/// available.
	///
	/// Because UDP is a connectionless protocol, the remote address
	/// and port will be recorded in the \a address and \a port
	/// parameters.
	///
	/// \param packet The packet where received data will be placed into
	/// \param address The reference variable the remote address will be placed into
	/// \param port The reference variable where the remote port will be placed into
	///
	/// \return The status of the receive task (Status::Done if successful, or Status::NotReady if no data is present in non-blocking mode)
	///
	///////////////////////////////////////////////////////////
	Status receive(Packet& packet, IpAddress& address, Uint16& port);

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
	/// \copydoc Socket::isBlocking
	///
	///////////////////////////////////////////////////////////
	bool isBlocking() const override;

private:
	sf::UdpSocket m_socket;
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::UdpSocket
/// \ingroup Network
///
/// This socket uses the UDP protocol to communicate data over
/// the internet. Data sent using this protocol is always sent
/// in data blocks, rather than streams, so each send() call will
/// always correspond to a receive(), assuming that the data wasn't
/// lost. The UDP protocol is faster compared to TCP, but
/// it is less reliable, and it is a connectionless protocol,
/// meaning that no connection has to be established before
/// communicating. In terms of reliability, datagrams sent using
/// the UDP protocol can be duplicated, lost, or arrive in a
/// different order they were sent. But the data that arrives
/// can't be corrupted. The maximum allowed size of a datagram
/// is defined by UdpSocket::MaxDatagramSize.
///
/// Because of the speed of UDP, it is best used in cases where
/// fast, real-time data is needed, and a small amount of loss
/// isn't too important, such as games, audio, and video.
///
/// To use a UDP socket, bind() the socket to a port number,
/// or let use any available port number by using Socket::AnyPort.
/// Then data can be sent and received to any remote peer by
/// send() and receive(). Because UDP is connectionless,
/// the ip address and port must be specified every time data
/// is sent, and it is received along with any received data.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// UdpSocket socket;
/// if (socket.bind(50000) != Status::Done)
///		// Failed to bind...
///
/// // Send data using a packet
/// Packet packet;
/// packet << 50 << "Hello World!";
///
/// // Send data to the port 6000 (just assume that another socket is attached there)
/// socket.send(packet, "localhost", 6000);
///
/// \endcode
///
///////////////////////////////////////////////////////////