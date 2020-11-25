#include <poly/Core/Logger.h>
#include <poly/Core/TypeInfo.h>

namespace poly
{


///////////////////////////////////////////////////////////
template <typename T>
inline void VertexBuffer::create(const std::vector<T>& data, BufferUsage usage, GLType dtype)
{
	if (data.size())
		create(&data[0], data.size(), usage, dtype);
}


///////////////////////////////////////////////////////////
template <typename T>
inline void VertexBuffer::create(const T* data, Uint32 num, BufferUsage usage, GLType dtype)
{
	// Make sure parameters are valid
	if (!num)
	{
		LOG_WARNING("Skipping vertex buffer creation");
		return;
	}

	// Buffer data
	bufferData(data, num * sizeof(T), usage);

	// Set the correct data type
	if (dtype == GLType::Unknown)
		setDataType<T>();
	else
		m_type = dtype;
}


///////////////////////////////////////////////////////////
template <typename T>
inline void VertexBuffer::update(const std::vector<T>& data, Uint32 offset)
{
	if (data.size())
		update(&data[0], data.size(), offset);
}


///////////////////////////////////////////////////////////
template <typename T>
inline void VertexBuffer::update(const T* data, Uint32 num, Uint32 offset)
{
	// Make sure parameters are valid
	if (!m_id || !data || !num || (num + offset) * sizeof(T) > m_size)
	{
		LOG_WARNING("Skipping vertex buffer update");
		return;
	}

	// Buffer subdata
	bufferSubData(data, num * sizeof(T), offset * sizeof(T));
}


///////////////////////////////////////////////////////////
template <typename T>
inline void VertexBuffer::setDataType()
{
	m_type = GLType::Float;
}


///////////////////////////////////////////////////////////
template <>
inline void VertexBuffer::setDataType<Int8>()
{
	m_type = GLType::Int8;
}


///////////////////////////////////////////////////////////
template <>
inline void VertexBuffer::setDataType<Uint8>()
{
	m_type = GLType::Uint8;
}


///////////////////////////////////////////////////////////
template <>
inline void VertexBuffer::setDataType<Int16>()
{
	m_type = GLType::Int16;
}


///////////////////////////////////////////////////////////
template <>
inline void VertexBuffer::setDataType<Uint16>()
{
	m_type = GLType::Uint16;
}


///////////////////////////////////////////////////////////
template <>
inline void VertexBuffer::setDataType<Int32>()
{
	m_type = GLType::Int32;
}


///////////////////////////////////////////////////////////
template <>
inline void VertexBuffer::setDataType<Uint32>()
{
	m_type = GLType::Uint32;
}


///////////////////////////////////////////////////////////
template <>
inline void VertexBuffer::setDataType<double>()
{
	m_type = GLType::Double;
}


}