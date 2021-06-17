#include <poly/Network/TcpSocket.h>

namespace poly
{


///////////////////////////////////////////////////////////
TcpSocket::TcpSocket()
{

}


///////////////////////////////////////////////////////////
Socket::Status TcpSocket::connect(const IpAddress& address, Uint16 port, Time timeout)
{
	return (Status)m_socket.connect(address.m_address, port, sf::microseconds(timeout.toMicroseconds()));
}


///////////////////////////////////////////////////////////
void TcpSocket::disconnect()
{
	m_socket.disconnect();
}


///////////////////////////////////////////////////////////
Socket::Status TcpSocket::send(const void* data, Uint32 size)
{
	return (Status)m_socket.send(data, size);
}


///////////////////////////////////////////////////////////
Socket::Status TcpSocket::send(const void* data, Uint32 size, Uint32& sent)
{
	return (Status)m_socket.send(data, size, sent);
}


///////////////////////////////////////////////////////////
Socket::Status TcpSocket::send(Packet& packet)
{
	return (Status)m_socket.send(packet.m_packet);
}


///////////////////////////////////////////////////////////
Socket::Status TcpSocket::receive(void* data, Uint32 size, Uint32& received)
{
	return (Status)m_socket.receive(data, size, received);
}


///////////////////////////////////////////////////////////
Socket::Status TcpSocket::receive(Packet& packet)
{
	return (Status)m_socket.receive(packet.m_packet);
}


///////////////////////////////////////////////////////////
void TcpSocket::setBlocking(bool blocking)
{
	m_socket.setBlocking(blocking);
}


///////////////////////////////////////////////////////////
Uint16 TcpSocket::getLocalPort() const
{
	return m_socket.getLocalPort();
}


///////////////////////////////////////////////////////////
IpAddress TcpSocket::getRemoteAddress() const
{
	return IpAddress(m_socket.getRemoteAddress().toInteger());
}


///////////////////////////////////////////////////////////
Uint16 TcpSocket::getRemotePort() const
{
	return m_socket.getRemotePort();
}


///////////////////////////////////////////////////////////
bool TcpSocket::isBlocking() const
{
	return m_socket.isBlocking();
}


}