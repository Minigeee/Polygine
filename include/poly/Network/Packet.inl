#include <poly/Core/Endian.h>

#include <poly/Math/Quaternion.h>
#include <poly/Math/Vector2.h>
#include <poly/Math/Vector3.h>
#include <poly/Math/Vector4.h>

namespace poly
{


///////////////////////////////////////////////////////////
template <typename T>
inline Packet::Packet(const T& data)
{
	*this << data;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Packet& operator>>(Packet& packet, T& data)
{
	packet.m_packet >> data;

	return packet;
}


///////////////////////////////////////////////////////////
template <>
inline Packet& operator>>(Packet& packet, Int64& data)
{
	if (packet.m_packet.getReadPosition() + sizeof(Int64) <= packet.m_packet.getDataSize())
	{
		const Uint8* packetData = (const Uint8*)packet.m_packet.getData();

		// Read data into an SFML int
		union
		{
			sf::Int64 i64;
			Uint8 bytes[8];
		} temp;
		packet.m_packet >> temp.i64;

		// Swap bytes back if the format is big endian
		if (Endian::getType() == Endian::Big)
		{
			data =
				(static_cast<Int64>(temp.bytes[0]) << 56) |
				(static_cast<Int64>(temp.bytes[1]) << 48) |
				(static_cast<Int64>(temp.bytes[2]) << 40) |
				(static_cast<Int64>(temp.bytes[3]) << 32) |
				(static_cast<Int64>(temp.bytes[4]) << 24) |
				(static_cast<Int64>(temp.bytes[5]) << 16) |
				(static_cast<Int64>(temp.bytes[6]) << 8) |
				(static_cast<Int64>(temp.bytes[7]));
		}
		else
			// Otherwise copy into poly int
			data = temp.i64;
	}

	return packet;
}


///////////////////////////////////////////////////////////
template <>
inline Packet& operator>>(Packet& packet, Uint64& data)
{
	if (packet.m_packet.getReadPosition() + sizeof(Uint64) <= packet.m_packet.getDataSize())
	{
		const Uint8* packetData = (const Uint8*)packet.m_packet.getData();

		// Read data into an SFML int
		union
		{
			sf::Uint64 i64;
			Uint8 bytes[8];
		} temp;
		packet.m_packet >> temp.i64;

		// Swap bytes back if the format is big endian
		if (Endian::getType() == Endian::Big)
		{
			data =
				(static_cast<Uint64>(temp.bytes[0]) << 56) |
				(static_cast<Uint64>(temp.bytes[1]) << 48) |
				(static_cast<Uint64>(temp.bytes[2]) << 40) |
				(static_cast<Uint64>(temp.bytes[3]) << 32) |
				(static_cast<Uint64>(temp.bytes[4]) << 24) |
				(static_cast<Uint64>(temp.bytes[5]) << 16) |
				(static_cast<Uint64>(temp.bytes[6]) << 8) |
				(static_cast<Uint64>(temp.bytes[7]));
		}
		else
			// Otherwise copy into poly int
			data = temp.i64;
	}

	return packet;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Packet& operator>>(Packet& packet, Vector2<T>& data)
{
	packet >> data.x >> data.y;
	return packet;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Packet& operator>>(Packet& packet, Vector3<T>& data)
{
	packet >> data.x >> data.y >> data.z;
	return packet;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Packet& operator>>(Packet& packet, Vector4<T>& data)
{
	packet >> data.x >> data.y >> data.z >> data.w;
	return packet;
}


///////////////////////////////////////////////////////////
template <>
inline Packet& operator>>(Packet& packet, Quaternion& data)
{
	packet >> data.x >> data.y >> data.z >> data.w;
	return packet;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Packet& operator<<(Packet& packet, const T& data)
{
	packet.m_packet << data;
	return packet;
}


///////////////////////////////////////////////////////////
template <>
inline Packet& operator<<(Packet& packet, const Int64& data)
{
	// Swap bytes if machine format is little endian
	if (Endian::getType() == Endian::Little)
	{
		Uint8 toWrite[] =
		{
			static_cast<Uint8>((data >> 56) & 0xFF),
			static_cast<Uint8>((data >> 48) & 0xFF),
			static_cast<Uint8>((data >> 40) & 0xFF),
			static_cast<Uint8>((data >> 32) & 0xFF),
			static_cast<Uint8>((data >> 24) & 0xFF),
			static_cast<Uint8>((data >> 16) & 0xFF),
			static_cast<Uint8>((data >> 8) & 0xFF),
			static_cast<Uint8>((data) & 0xFF)
		};
		packet.m_packet.append(&toWrite, sizeof(toWrite));
	}
	else
		// Otherwise, copy without swapping
		packet.m_packet.append(&data, sizeof(Int64));

	return packet;
}


///////////////////////////////////////////////////////////
template <>
inline Packet& operator<<(Packet& packet, const Uint64& data)
{
	// Swap bytes if machine format is little endian
	if (Endian::getType() == Endian::Little)
	{
		Uint8 toWrite[] =
		{
			static_cast<Uint8>((data >> 56) & 0xFF),
			static_cast<Uint8>((data >> 48) & 0xFF),
			static_cast<Uint8>((data >> 40) & 0xFF),
			static_cast<Uint8>((data >> 32) & 0xFF),
			static_cast<Uint8>((data >> 24) & 0xFF),
			static_cast<Uint8>((data >> 16) & 0xFF),
			static_cast<Uint8>((data >> 8) & 0xFF),
			static_cast<Uint8>((data) & 0xFF)
		};
		packet.m_packet.append(&toWrite, sizeof(toWrite));
	}
	else
		// Otherwise, copy without swapping
		packet.m_packet.append(&data, sizeof(Uint64));

	return packet;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Packet& operator<<(Packet& packet, const Vector2<T>& data)
{
	packet << data.x << data.y;
	return packet;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Packet& operator<<(Packet& packet, const Vector3<T>& data)
{
	packet << data.x << data.y << data.z;
	return packet;
}


///////////////////////////////////////////////////////////
template <typename T>
inline Packet& operator<<(Packet& packet, const Vector4<T>& data)
{
	packet << data.x << data.y << data.z << data.w;
	return packet;
}


///////////////////////////////////////////////////////////
template <>
inline Packet& operator<<(Packet& packet, const Quaternion& data)
{
	packet << data.x << data.y << data.z << data.w;
	return packet;
}


}