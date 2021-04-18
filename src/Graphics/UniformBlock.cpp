#include <poly/Graphics/UniformBlock.h>

namespace poly
{


///////////////////////////////////////////////////////////
UniformBlock::UniformBlock() :
	m_data				(0),
	m_offset			(0),
	m_prevOffset		(0),
	m_blockSize			(0),
	m_bufferReset		(false)
{

}


///////////////////////////////////////////////////////////
UniformBlock::~UniformBlock()
{
	if (m_data)
		free(m_data);

	m_data = 0;
}


///////////////////////////////////////////////////////////
void UniformBlock::bind(Uint32 index)
{
	// Bind the buffer range to the specified index
	m_vertexBuffer.bind(BufferTarget::Uniform, index, m_prevOffset, m_offset - m_prevOffset);
}


///////////////////////////////////////////////////////////
void UniformBlock::update()
{
	// Round the block size to the next 4N alignment
	m_blockSize = (m_blockSize + 15) / 16 * 16;

	// Stream data into uniform buffer
	MapBufferFlags flags = MapBufferFlags::Write | MapBufferFlags::Unsynchronized;

	// Invalidate buffer if it was reset
	if (m_bufferReset)
	{
		flags |= MapBufferFlags::InvalidateBuffer;
		m_bufferReset = false;
	}

	// Map buffer
	m_vertexBuffer.setTarget(BufferTarget::Uniform);
	void* buffer = m_vertexBuffer.map(m_offset, m_blockSize, flags);

	// Copy data
	memcpy(buffer, m_data + m_offset, m_blockSize);

	// Unmap buffer
	m_vertexBuffer.unmap();

	// Update buffer offset
	m_prevOffset = m_offset;
	m_offset += m_blockSize;
	m_blockSize = 0;
}


///////////////////////////////////////////////////////////
void UniformBlock::setBufferSize(Uint32 size)
{
	// Allocate new data on CPU side
	if (m_data)
		free(m_data);
	m_data = (Uint8*)malloc(size);

	// Create a new uniform buffer
	m_vertexBuffer.create((Uint8*)NULL, size, BufferUsage::Stream, GLType::Float);
}


///////////////////////////////////////////////////////////
Uint32 UniformBlock::getId() const
{
	return m_vertexBuffer.getId();
}


///////////////////////////////////////////////////////////
Uint32 UniformBlock::getBufferSize() const
{
	return m_vertexBuffer.getSize();
}


///////////////////////////////////////////////////////////
Uint32 UniformBlock::getBlockSize() const
{
	return m_blockSize;
}


}