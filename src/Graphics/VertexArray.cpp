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
	m_drawMode			(DrawMode::Triangles),
	m_hasElementBuffer	(false)
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
	}
}


///////////////////////////////////////////////////////////
void VertexArray::unbind()
{
	if (s_currentBound == m_id)
	{
		glCheck(glBindVertexArray(0));
		s_currentBound = 0;
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
void VertexArray::draw(Uint32 instances, Uint32 offset)
{
	// Make sure array is bound
	bind();

	// Choose the correct draw function
	if (m_hasElementBuffer)
	{
		// Use the element buffer
		if (instances == 1)
			glCheck(glDrawElements((GLenum)m_drawMode, m_numVertices - offset, GL_UNSIGNED_INT, (const void*)offset));
		else
			glCheck(glDrawElementsInstanced((GLenum)m_drawMode, m_numVertices - offset, GL_UNSIGNED_INT, (const void*)offset, instances));
	}
	else
	{
		// Use regular array buffers
		if (instances == 1)
			glCheck(glDrawArrays((GLenum)m_drawMode, offset, m_numVertices - offset));
		else
			glCheck(glDrawArraysInstanced((GLenum)m_drawMode, offset, m_numVertices - offset, instances));
	}
}


///////////////////////////////////////////////////////////
void VertexArray::setNumVertices(Uint32 numVertices)
{
	m_numVertices = numVertices;
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
	m_hasElementBuffer = true;

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
DrawMode VertexArray::getDrawMode() const
{
	return m_drawMode;
}


///////////////////////////////////////////////////////////
bool VertexArray::hasElementBuffer() const
{
	return m_hasElementBuffer;
}


}