#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/UniformBuffer.h>

namespace poly
{


///////////////////////////////////////////////////////////
Uint32 UniformBuffer::s_uniformBlockAlignment = 0;


///////////////////////////////////////////////////////////
UniformBuffer::UniformBuffer() :
	m_offset			(0),
	m_prevOffset		(0),
	m_prevSize			(0)
{

}


///////////////////////////////////////////////////////////
Uint32 UniformBuffer::getUniformBlockAlignment()
{
	if (!s_uniformBlockAlignment)
	{
		int align = 0;
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &align);
		s_uniformBlockAlignment = align;
	}

	return s_uniformBlockAlignment;
}


///////////////////////////////////////////////////////////
void UniformBuffer::bind(Uint32 index, Uint32 offset, Uint32 size)
{
	if (offset == 0xFFFFFFFFu)
		offset = m_prevOffset;
	if (size == 0)
		size = m_prevSize;

	// Bind the buffer range to the specified index
	m_uniformBuffer.bind(BufferTarget::Uniform, index, offset, size);
}


///////////////////////////////////////////////////////////
void UniformBuffer::setSize(Uint32 size)
{
	// Create a new uniform buffer
	m_uniformBuffer.create((Uint8*)NULL, size, BufferUsage::Stream, GLType::Float);
	m_uniformBuffer.setTarget(BufferTarget::Uniform);
}


///////////////////////////////////////////////////////////
Uint32 UniformBuffer::getId() const
{
	return m_uniformBuffer.getId();
}


///////////////////////////////////////////////////////////
Uint32 UniformBuffer::getSize() const
{
	return m_uniformBuffer.getSize();
}


}