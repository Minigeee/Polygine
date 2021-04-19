
namespace poly
{


///////////////////////////////////////////////////////////
template <typename T>
inline Uint32 UniformBuffer::pushData(const T& data)
{
	// Can't push data if the uniform buffer hasn't been created
	if (!m_uniformBuffer.getId())
		return 0xFFFFFFFFu;

	// Get map flags
	MapBufferFlags flags = MapBufferFlags::Write | MapBufferFlags::Unsynchronized | MapBufferFlags::InvalidateRange;

	// Reset if the push will cause overflow
	if (m_offset + sizeof(T) > m_uniformBuffer.getSize())
		m_offset = 0;

	// Update the previous offset and size
	m_prevOffset = m_offset;
	m_prevSize = sizeof(T);

	// Map buffer
	T* buffer = (T*)m_uniformBuffer.map(m_offset, sizeof(T), flags);

	// Copy data
	*buffer = data;

	// Unmap
	m_uniformBuffer.unmap();

	// Update and align offset
	Uint32 align = getUniformBlockAlignment();
	m_offset += sizeof(T);
	m_offset = (m_offset + align - 1) / align * align;

	return m_prevOffset;
}


}