#ifndef POLY_IP_ADDRESS_H
#define POLY_IP_ADDRESS_H

#include <poly/Core/DataTypes.h>
#include <poly/Core/Time.h>

#include <SFML/Network/IpAddress.hpp>

namespace poly
{


class TcpListener;
class TcpSocket;
class UdpSocket;


///////////////////////////////////////////////////////////
/// \brief Holds information for a IPv4 network address
///
///////////////////////////////////////////////////////////
class IpAddress
{
	friend TcpListener;
	friend TcpSocket;
	friend UdpSocket;

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Creates an empty address
	///
	///////////////////////////////////////////////////////////
	IpAddress();

	///////////////////////////////////////////////////////////
	/// \brief Create an address from a string
	///
	/// The string address can be a decimal address, a network name,
	/// or URL. Examples:
	///
	/// * "127.0.0.1"
	/// * "localhost"
	/// * "www.google.com"
	///
	/// \param address The string address
	///
	///////////////////////////////////////////////////////////
	IpAddress(const std::string& address);

	///////////////////////////////////////////////////////////
	/// \copydoc IpAddress(const std::string&)
	///
	///////////////////////////////////////////////////////////
	IpAddress(const char* address);

	///////////////////////////////////////////////////////////
	/// \brief Create an address from the 4 bytes
	///
	/// Creating an aaddress from the 4 bytes is equivalent to
	/// creating an adress with "x.x.x.x"
	///
	/// \param byte1 The 1st byte
	/// \param byte2 The 2nd byte
	/// \param byte3 The 3rd byte
	/// \param byte4 The 4th byte
	///
	///////////////////////////////////////////////////////////
	IpAddress(Uint8 byte1, Uint8 byte2, Uint8 byte3, Uint8 byte4);

	///////////////////////////////////////////////////////////
	/// \brief Create an address from a 32-bit integer
	///
	/// \param address The 32-bit address
	///
	///////////////////////////////////////////////////////////
	IpAddress(Uint32 address);

	///////////////////////////////////////////////////////////
	/// \brief Get the string representation of the ip address
	///
	/// \return The string representation of the address
	///
	///////////////////////////////////////////////////////////
	std::string toString() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the integer representation of the ip address
	///
	/// \return The integer representation of the address
	///
	///////////////////////////////////////////////////////////
	Uint32 toInteger() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the ip address of the machine from the LAN point of view
	///
	/// This address is only useful for local network communication.
	/// This function can execute quickly unlike getPublicAddress().
	///
	/// \return The local ip address
	///
	///////////////////////////////////////////////////////////
	static IpAddress getLocalAddress();

	///////////////////////////////////////////////////////////
	/// \brief Get the ip address of the machine from the internet point of view
	///
	/// This function may take a while to execute because it depends on
	/// the network connection and a remote server to retreive the
	/// machine's public address. So it shouldn't be used too often
	/// or in time sensitive loops. It is possible to cancel the
	/// function execution by setting a timeout value. A timeout
	/// value of zero disables the timeout.
	///
	/// \return The public ip address
	///
	///////////////////////////////////////////////////////////
	static IpAddress getPublicAddress(Time timeout = Time());

	static const IpAddress None;		//!< An empty invalid ip address
	static const IpAddress Any;			//!< An address representing any address
	static const IpAddress LocalHost;	//!< The "localhost" address
	static const IpAddress Broadcast;	//!< The broadcast address (for sending UDP messages to everyone on a local network)

private:
	sf::IpAddress m_address;			//!< THe internal SFML ip address
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::IpAddress
/// \ingroup Network
///
/// Ip addresses are used to identify computers in a network.
/// This class holds data for IPv4 addresses, and can be used
/// to construct addresses conveniently.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// IpAddress a1("127.0.0.1");
/// IpAddress a2(127, 0, 0, 1);
/// IpAddress a3("localhost");
/// IpAddress a4("www.google.com");
///
/// \endcode
///
///////////////////////////////////////////////////////////