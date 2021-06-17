#ifndef POLY_PACKET_H
#define POLY_PACKET_H

#include <poly/Core/DataTypes.h>

#include <SFML/Network/Packet.hpp>

namespace poly
{


class Packet;
class TcpSocket;
class UdpSocket;


///////////////////////////////////////////////////////////
/// \brief Read data from a packet
///
/// Template specializations exist for all the common data types.
/// To add packet support for a custom data type, just add
/// a template specialization in another header file, providing
/// the correct instructions to read data from the packet.
///
/// Data is read from the packet in a queue like manner.
///
/// \param packet The packet to read data from
/// \param data The object to put the data into
///
/// \return A reference to the packet object to chain reads
///
///////////////////////////////////////////////////////////
template <typename T>
Packet& operator>>(Packet& packet, T& data);

///////////////////////////////////////////////////////////
/// \brief Write data into a packet
///
/// Template specializations exist for all the common data types.
/// To add packet support for a custom data type, just add
/// a template specialization in another header file, providing
/// the correct instructions to write data into the packet.
///
/// Data is written into the packet in a queue like manner.
///
/// \param packet The packet to write data into
/// \param data The object to take the data from
///
/// \return A reference to the packet object to chain writes
///
///////////////////////////////////////////////////////////
template <typename T>
Packet& operator<<(Packet& packet, const T& data);


///////////////////////////////////////////////////////////
/// \brief A utility class used to wrap data to send/receive through sockets
///
///////////////////////////////////////////////////////////
class Packet
{
	template <typename T>
	friend Packet& operator>>(Packet& packet, T& data);

	template <typename T>
	friend Packet& operator<<(Packet& packet, const T& data);

	friend TcpSocket;
	friend UdpSocket;

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Creates an empty packet
	///
	///////////////////////////////////////////////////////////
	Packet();

	///////////////////////////////////////////////////////////
	/// \brief Create a packet from an object
	///
	/// This constructor creates the object and uses the << operator
	/// to add data to the packet.
	///
	/// \param data The data to put in the packet
	///
	///////////////////////////////////////////////////////////
	template <typename T>
	Packet(const T& data);

	///////////////////////////////////////////////////////////
	/// \brief Check if the packet has any more data to read
	///
	/// \return True if there is still data left in the packet to read
	///
	///////////////////////////////////////////////////////////
	operator bool() const;

	///////////////////////////////////////////////////////////
	/// \brief Write data into the packet
	///
	/// This copies data directly into the packet buffer.
	/// It does not handle type sizes or endianness.
	///
	/// \param data A pointer to the data to write
	/// \param size The number of bytes to write into the packet
	///
	///////////////////////////////////////////////////////////
	void write(const void* data, Uint32 size);

	///////////////////////////////////////////////////////////
	/// \brief Remove all data from the packet
	///
	///////////////////////////////////////////////////////////
	void clear();

	///////////////////////////////////////////////////////////
	/// \brief Get a pointer to the packet data
	///
	/// \param offset The offset of the data to retrieve (in bytes)
	///
	/// \return A pointer to the packet data
	///
	///////////////////////////////////////////////////////////
	void* getData(Uint32 offset = 0) const;

	///////////////////////////////////////////////////////////
	/// \brief Get the size of the packet data in bytes
	///
	/// \return Get the size of the packet data in bytes
	///
	///////////////////////////////////////////////////////////
	Uint32 getSize() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the current reading offset
	///
	/// \return The offset where data is read from the packet buffer
	///
	///////////////////////////////////////////////////////////
	Uint32 getReadOffset() const;

	///////////////////////////////////////////////////////////
	/// \brief Check if the reading offset is at the end of the packet and data is done being read
	///
	/// \return True if the reading offset is at the end of the packet
	///
	///////////////////////////////////////////////////////////
	bool endOfPacket() const;

private:
	sf::Packet m_packet;			//!< The internal SFML packet
};


}

#include <poly/Network/Packet.inl>

#endif


///////////////////////////////////////////////////////////
/// \class poly::Packet
/// \ingroup Network
///
/// A packet provides utility for transferring data over the internet.
/// When data is sent over the internet, other machines will be
/// handling it, and things such as data type size and endianness will
/// not be consistent across all machines. Packets handle this,
/// and provide an easier interface to package data to be sent.
///
/// To add data to a packet, use the << operator, and to read data
/// from the packet, use the >> operator. Data is added and read
/// from the packet in a queue-like manner, so data that is addded
/// first will be read first. The write and read operators provide
/// template specializations for all the common data types. To
/// add a custom data type, make a new template specialization
/// for both the >> and << operators.
///
/// Usage example:
/// \code
///
/// using namespce poly;
///
/// Packet packet;
///
/// // Write some data into the packets
/// packet << 3.1415f << 314;
///
/// float a;
/// int b;
/// packet >> a >> b;
///
///
/// // Custom data type
/// struct DataType
/// {
///		Vector3f m_position;
///		Quaternion m_rotation;
/// };
///
/// // Template specializations
/// template <>
/// inline operator>>(Packet& packet, DataType& data)
/// {
///		packet >> data.m_position >> data.m_rotation;
/// }
///
/// template <>
/// inline operator<<(Packet& packet, const DataType& data)
/// {
///		packet << data.m_position << data.m_rotation;
/// }
///
/// // These specializations can be used now
/// DataType dt1;
/// dt1.m_position = Vector3f(0.0f, 1.0f, 10.0f);
/// packet << dt1;
///
///	DataType dt2;
/// packet >> dt2;
///
/// \endcode
///
///////////////////////////////////////////////////////////