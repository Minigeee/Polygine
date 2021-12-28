#include <poly/Network/UdpSocket.h>

namespace poly
{


///////////////////////////////////////////////////////////
UdpSocket::UdpSocket()
{

}


///////////////////////////////////////////////////////////
Socket::Status UdpSocket::bind(Uint16 port, const IpAddress& address)
{
	return (Status)m_socket.bind(port, address.m_address);
}


///////////////////////////////////////////////////////////
void UdpSocket::unbind()
{
	m_socket.unbind();
}


///////////////////////////////////////////////////////////
Socket::Status UdpSocket::send(const void* data, Uint32 size, const IpAddress& address, Uint16 port)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	return (Status)m_socket.send(data, size, address.m_address, port);
}


///////////////////////////////////////////////////////////
Socket::Status UdpSocket::send(Packet& packet, const IpAddress& address, Uint16 port)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	return (Status)m_socket.send(packet.m_packet, address.m_address, port);
}


///////////////////////////////////////////////////////////
Socket::Status UdpSocket::receive(void* data, Uint32 max, Uint32& received, IpAddress& address, Uint16& port)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	return (Status)m_socket.receive(data, max, received, address.m_address, port);
}


///////////////////////////////////////////////////////////
Socket::Status UdpSocket::receive(Packet& packet, IpAddress& address, Uint16& port)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	return (Status)m_socket.receive(packet.m_packet, address.m_address, port);
}


///////////////////////////////////////////////////////////
void UdpSocket::setBlocking(bool blocking)
{
	m_socket.setBlocking(blocking);
}


///////////////////////////////////////////////////////////
Uint16 UdpSocket::getLocalPort() const
{
	return m_socket.getLocalPort();
}


///////////////////////////////////////////////////////////
bool UdpSocket::isBlocking() const
{
	return m_socket.isBlocking();
}


}