#include <poly/Network/TcpListener.h>

namespace poly
{


///////////////////////////////////////////////////////////
TcpListener::TcpListener()
{

}


///////////////////////////////////////////////////////////
Socket::Status TcpListener::listen(Uint16 port, const IpAddress& address)
{
	return (Status)m_listener.listen(port, address.m_address);
}


///////////////////////////////////////////////////////////
void TcpListener::close()
{
	m_listener.close();
}


///////////////////////////////////////////////////////////
Socket::Status TcpListener::accept(TcpSocket& socket)
{
	return (Status)m_listener.accept(socket.m_socket);
}


///////////////////////////////////////////////////////////
void TcpListener::setBlocking(bool blocking)
{
	m_listener.setBlocking(blocking);
}


///////////////////////////////////////////////////////////
Uint16 TcpListener::getLocalPort() const
{
	return m_listener.getLocalPort();
}


///////////////////////////////////////////////////////////
bool TcpListener::isBlocking() const
{
	return m_listener.isBlocking();
}


}