#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/VertexBuffer.h>

namespace poly
{


///////////////////////////////////////////////////////////
Uint32 VertexBuffer::currentBound[4] = { 0, 0, 0, 0 };


///////////////////////////////////////////////////////////
GLenum targetToGLEnum(BufferTarget target)
{
	switch (target)
	{
	case BufferTarget::Array:
		return GL_ARRAY_BUFFER;
	case BufferTarget::Element:
		return GL_ELEMENT_ARRAY_BUFFER;
	case BufferTarget::TransformFeedback:
		return GL_TRANSFORM_FEEDBACK_BUFFER;
	case BufferTarget::Uniform:
		return GL_UNIFORM_BUFFER;
	default:
		return 0;
	}
}


///////////////////////////////////////////////////////////
VertexBuffer::VertexBuffer() :
	m_id		(0),
	m_size		(0),
	m_usage		(BufferUsage::Static),
	m_target	(BufferTarget::Array),
	m_type		(GLType::Unknown)
{

}


///////////////////////////////////////////////////////////
VertexBuffer::~VertexBuffer()
{
	if (m_id)
		glCheck(glDeleteBuffers(1, &m_id));

	m_id = 0;
}


///////////////////////////////////////////////////////////
void VertexBuffer::bind()
{
	// Bind to current target
	bind(m_target);
}


///////////////////////////////////////////////////////////
void VertexBuffer::bind(BufferTarget target)
{
	// Create a buffer if needed
	if (!m_id)
		glCheck(glGenBuffers(1, &m_id));

	// Only bind if id exists and is not already bound to the target
	if (currentBound[(Uint32)target] != m_id)
	{
		// Unbind previous
		if (currentBound[(Uint32)m_target] == m_id)
			currentBound[(Uint32)m_target] = 0;

		// Bind buffer
		glCheck(glBindBuffer(targetToGLEnum(target), m_id));

		// Update the current bound buffer
		m_target = target;
		currentBound[(Uint32)m_target] = m_id;
	}
}


///////////////////////////////////////////////////////////
void VertexBuffer::bind(BufferTarget target, Uint32 index)
{
	// Create a buffer if needed
	if (!m_id)
		glCheck(glGenBuffers(1, &m_id));

	// Only certain targets work with bindBufferBase
	bool targetValid = target == BufferTarget::TransformFeedback || target == BufferTarget::Uniform;

	// Only bind if id exists and is not already bound to the target
	if (m_id && targetValid && currentBound[(Uint32)target] != m_id)
	{
		// Unbind previous
		if (currentBound[(Uint32)m_target] == m_id)
			currentBound[(Uint32)m_target] = 0;

		// Bind buffer
		glCheck(glBindBufferBase(targetToGLEnum(target), index, m_id));

		// Update the current bound buffer
		m_target = target;
		currentBound[(Uint32)m_target] = m_id;
	}
}


///////////////////////////////////////////////////////////
void VertexBuffer::update(const VertexBuffer& buffer, Uint32 offset)
{
	if (&buffer != this)
	{
		// Check for valid parameters
		if (!m_id || !buffer.m_id || !buffer.m_size || m_size - offset < buffer.m_size)
		{
			LOG_WARNING("Skipping vertex buffer update");
			return;
		}

		// Bind buffers
		glCheck(glBindBuffer(GL_COPY_READ_BUFFER, buffer.m_id));
		glCheck(glBindBuffer(GL_COPY_WRITE_BUFFER, m_id));

		// Buffer data
		glCheck(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, offset, buffer.m_size));
	}
}


///////////////////////////////////////////////////////////
void VertexBuffer::bufferData(const void* data, Uint32 size, BufferUsage usage)
{
	// Ensure buffer is bound
	bind();

	// Buffer data
	glCheck(glBufferData(targetToGLEnum(m_target), size, data, (GLenum)usage));

	// Update data
	m_size = size;
	m_usage = usage;
}


///////////////////////////////////////////////////////////
void VertexBuffer::bufferSubData(const void* data, Uint32 size, Uint32 offset)
{
	// Ensure buffer is bound
	bind();

	// Buffer data
	glCheck(glBufferSubData(targetToGLEnum(m_target), offset, size, data));
}


///////////////////////////////////////////////////////////
void* VertexBuffer::map(Uint32 offset, Uint32 size, int flags)
{
	// Ensure buffer is bound
	bind();

	// Map data
	void* map = 0;
	glCheck(map = glMapBufferRange(targetToGLEnum(m_target), offset, size, flags));

	return map;
}


///////////////////////////////////////////////////////////
void VertexBuffer::unmap()
{
	// Ensure buffer is bound
	bind();

	// Unmap buffer
	glUnmapBuffer(targetToGLEnum(m_target));
}


///////////////////////////////////////////////////////////
void VertexBuffer::setTarget(BufferTarget target)
{
	m_target = target;
}


///////////////////////////////////////////////////////////
Uint32 VertexBuffer::getId() const
{
	return m_id;
}


///////////////////////////////////////////////////////////
Uint32 VertexBuffer::getSize() const
{
	return m_size;
}


///////////////////////////////////////////////////////////
BufferUsage VertexBuffer::getUsage() const
{
	return m_usage;
}


///////////////////////////////////////////////////////////
BufferTarget VertexBuffer::getTarget() const
{
	return m_target;
}


///////////////////////////////////////////////////////////
GLType VertexBuffer::getDataType() const
{
	return m_type;
}


}