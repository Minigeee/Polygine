#include <poly/Core/Allocate.h>
#include <poly/Core/RingBuffer.h>

namespace poly
{


///////////////////////////////////////////////////////////
RingBuffer::RingBuffer() :
	m_buffer		(0),
	m_front			(0),
	m_size			(0),
	m_capacity		(0)
{

}


///////////////////////////////////////////////////////////
RingBuffer::~RingBuffer()
{
	if (m_buffer)
		FREE_DBG(m_buffer);

	m_buffer = 0;
	m_front = 0;
}


///////////////////////////////////////////////////////////
Uint32 RingBuffer::read(void* buffer, Uint32 max)
{
	// Return 0 if the buffer isn't allocated or the size is 0
	if (!m_buffer || !m_size)
		return 0;

	// Check how many bytes can be read
	Uint32 numRead = max > m_size ? m_size : max;

	// Read the desired amount of data into the output buffer
	Uint32 roffset = m_capacity - ((Uint8*)m_front - (Uint8*)m_buffer);
	if (roffset < numRead)
	{
		// The data loops around, 2 copy operations are needed
		memcpy(buffer, m_front, roffset);
		memcpy((Uint8*)buffer + roffset, m_buffer, numRead - roffset);
	}
	else
		// The data does not loop around, the read can be performed with a single copy
		memcpy(buffer, m_front, numRead);

	// Update the size of the data in the buffer
	m_size -= numRead;

	// Advance the front pointer
	if (m_size == 0)
		// Reset the front pointer if the size is 0
		m_front = m_buffer;

	else
	{
		Uint32 offset = (Uint8*)m_front - (Uint8*)m_buffer;
		offset = (offset + numRead) % m_capacity;
		m_front = (Uint8*)m_buffer + offset;
	}

	return numRead;
}


///////////////////////////////////////////////////////////
void RingBuffer::write(const void* data, Uint32 size)
{
	// Update size of the buffer
	m_size += size;

	// Reallocate the buffer if needed
	if (m_size > m_capacity)
	{
		// Resize to twice the current size
		Uint32 newSize = 2 * m_size;
		Uint8* newData = (Uint8*)MALLOC_DBG(newSize);

		// Copy old data
		Uint32 roffset = m_capacity - ((Uint8*)m_front - (Uint8*)m_buffer);
		memcpy(newData, m_front, roffset);
		memcpy(newData + roffset, m_buffer, m_capacity - roffset);

		// Free old data
		FREE_DBG(m_buffer);

		// Update pointers
		m_buffer = newData;
		m_front = m_buffer;
		m_capacity = newSize;
	}

	// Get the location to insert data
	Uint32 backOffset = ((Uint8*)m_front - (Uint8*)m_buffer) + (m_size - size);
	backOffset %= m_capacity;

	// Check if this will cause the data to loop around
	Uint32 roffset = m_capacity - backOffset;
	if (roffset < size)
	{
		// The data loops around, 2 copy operations are needed
		memcpy((Uint8*)m_buffer + backOffset, data, roffset);
		memcpy(m_buffer, data, size - roffset);
	}
	else
		// The data does not loop around, the read can be performed with a single copy
		memcpy((Uint8*)m_buffer + backOffset, data, size);
}


///////////////////////////////////////////////////////////
void RingBuffer::clear()
{
	// Reset size and pointers
	m_size = 0;
	m_front = m_buffer;
}


///////////////////////////////////////////////////////////
Uint32 RingBuffer::size() const
{
	return m_size;
}


///////////////////////////////////////////////////////////
Uint32 RingBuffer::capacity() const
{
	return m_capacity;
}


}