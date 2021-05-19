#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/VertexArray.h>

namespace poly
{


///////////////////////////////////////////////////////////
Uint32 getTypeSize(GLType type)
{
	switch (type)
	{
	case GLType::Float:
	case GLType::Int32:
	case GLType::Uint32:
		return 4;

	case GLType::Int8:
	case GLType::Uint8:
		return 1;

	case GLType::Int16:
	case GLType::Uint16:
		return 2;

	case GLType::Double:
		return 8;

	default:
		return 0;
	}
}


///////////////////////////////////////////////////////////
Uint32 VertexArray::s_currentBound = 0;


///////////////////////////////////////////////////////////
VertexArray::VertexArray() :
	m_id				(0),
	m_numVertices		(0),
	m_vertexOffset		(0),
	m_drawMode			(DrawMode::Triangles),
	m_elementBuffer		(0)
{

}


///////////////////////////////////////////////////////////
VertexArray::~VertexArray()
{
	if (m_id)
		glCheck(glDeleteVertexArrays(1, &m_id));

	m_id = 0;
}


///////////////////////////////////////////////////////////
void VertexArray::bind()
{
	// Create the array if it doesn't exist
	if (!m_id)
		glCheck(glGenVertexArrays(1, &m_id));

	// Only bind if it already isn't bound
	if (s_currentBound != m_id)
	{
		glCheck(glBindVertexArray(m_id));
		s_currentBound = m_id;

		// Update element buffer
		VertexBuffer::s_currentBound[(Uint32)BufferTarget::Element] = m_elementBuffer;
	}
}


///////////////////////////////////////////////////////////
void VertexArray::unbind()
{
	if (s_currentBound == m_id)
	{
		glCheck(glBindVertexArray(0));
		s_currentBound = 0;

		// Update element buffer
		VertexBuffer::s_currentBound[(Uint32)BufferTarget::Element] = 0;
	}
}


///////////////////////////////////////////////////////////
void VertexArray::addBuffer(VertexBuffer& buffer, Uint32 index, Uint32 size, Uint32 stride, Uint32 offset, Uint32 divisor, GLType dtype)
{
	// Make sure parameters are good
	if (!buffer.getSize() || buffer.getDataType() == GLType::Unknown || !size)
	{
		LOG_ERROR("Skipping add vertex buffer to vertex array");
		return;
	}

	// Make sure vertex array is bound
	bind();

	// Make sure vertex buffer is bound
	buffer.bind();

	// Add attributes
	if (dtype == GLType::Unknown)
		dtype = buffer.getDataType();

	if (dtype == GLType::Float || dtype == GLType::Double)
		glCheck(glVertexAttribPointer(index, size, (GLenum)dtype, GL_FALSE, stride, (void*)offset));
	else
		glCheck(glVertexAttribIPointer(index, size, (GLenum)dtype, stride, (void*)offset));

	// Enable attrib
	glCheck(glEnableVertexAttribArray(index));
	// Enable instanced data if specified
	if (divisor)
		glCheck(glVertexAttribDivisor(index, divisor));

	// Get the number of vertices
	if (!m_numVertices)
	{
		if (stride)
			m_numVertices = buffer.getSize() / stride;

		else
			m_numVertices = buffer.getSize() / (size * getTypeSize(dtype));
	}
}


///////////////////////////////////////////////////////////
void VertexArray::draw(Uint32 instances, Uint32 offset, Uint32 vertices)
{
	// Make sure array is bound
	bind();

	// Decide which offset to use
	if (offset == 0xFFFFFFFF)
		offset = m_vertexOffset;
	if (vertices == 0xFFFFFFFF)
		vertices = m_numVertices;

	// Choose the correct draw function
	if (m_elementBuffer)
	{
		// Use the element buffer
		if (instances == 1)
			glCheck(glDrawElements((GLenum)m_drawMode, vertices, GL_UNSIGNED_INT, (const void*)(offset * sizeof(Uint32))));
		else
			glCheck(glDrawElementsInstanced((GLenum)m_drawMode, vertices, GL_UNSIGNED_INT, (const void*)(offset * sizeof(Uint32)), instances));
	}
	else
	{
		// Use regular array buffers
		if (instances == 1)
			glCheck(glDrawArrays((GLenum)m_drawMode, offset, vertices));
		else
			glCheck(glDrawArraysInstanced((GLenum)m_drawMode, offset, vertices, instances));
	}
}


///////////////////////////////////////////////////////////
void VertexArray::setNumVertices(Uint32 numVertices)
{
	m_numVertices = numVertices;
}


///////////////////////////////////////////////////////////
void VertexArray::setVertexOffset(Uint32 offset)
{
	m_vertexOffset = offset;
}


///////////////////////////////////////////////////////////
void VertexArray::setDrawMode(DrawMode mode)
{
	m_drawMode = mode;
}


///////////////////////////////////////////////////////////
void VertexArray::setElementBuffer(VertexBuffer& buffer)
{
	// Make sure array is bound
	bind();

	// Bind the buffer to the element target
	buffer.bind(BufferTarget::Element);
	m_elementBuffer = buffer.getId();

	// Unbind to save vertex array state
	unbind();
}


///////////////////////////////////////////////////////////
Uint32 VertexArray::getId() const
{
	return m_id;
}


///////////////////////////////////////////////////////////
Uint32 VertexArray::getNumVertices() const
{
	return m_numVertices;
}


///////////////////////////////////////////////////////////
Uint32 VertexArray::getVertexOffset() const
{
	return m_vertexOffset;
}


///////////////////////////////////////////////////////////
DrawMode VertexArray::getDrawMode() const
{
	return m_drawMode;
}


///////////////////////////////////////////////////////////
bool VertexArray::hasElementBuffer() const
{
	return (bool)m_elementBuffer;
}


}