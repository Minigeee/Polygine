#include <poly/Core/Allocate.h>
#include <poly/Core/Stream.h>

namespace poly
{


///////////////////////////////////////////////////////////
ReadStream::ReadStream()
{

}


///////////////////////////////////////////////////////////
ReadStream::~ReadStream()
{

}


///////////////////////////////////////////////////////////
void ReadStream::onReadData(const std::function<void(void*, Uint32)>& func)
{
	m_onReadData = func;
}


///////////////////////////////////////////////////////////
WriteStream::~WriteStream()
{

}


///////////////////////////////////////////////////////////
void WriteStream::onWriteData(const std::function<void(void*, Uint32)>& func)
{
	m_onWriteData = func;
}


///////////////////////////////////////////////////////////
BufferStream::BufferStream() :
	m_buffer		(0),
	m_front			(0),
	m_size			(0),
	m_capacity		(0),
	m_pipe			(0)
{

}


///////////////////////////////////////////////////////////
BufferStream::~BufferStream()
{
	if (m_buffer)
		FREE_DBG(m_buffer);

	m_buffer = 0;
	m_front = 0;
}


///////////////////////////////////////////////////////////
Uint32 BufferStream::read(void* buffer, Uint32 max)
{
	// Return null if the buffer doesn't exist or size is 0
	if (!m_buffer || !m_size)
		return 0;

	if (max > m_size)
		max = m_size;

	// Read the desired amount of data into the output buffer
	Uint32 roffset = m_capacity - ((Uint8*)m_front - (Uint8*)m_buffer);
	if (roffset < max)
	{
		// The data loops around, 2 copy operations are needed
		memcpy(buffer, m_front, roffset);
		memcpy((Uint8*)buffer + roffset, m_buffer, max - roffset);
	}
	else
		// The data does not loop around, the read can be performed with a single copy
		memcpy(buffer, m_front, max);

	// Update the size of the data in the buffer
	m_size -= max;

	// Advance the front pointer
	if (m_size == 0)
		// Reset the front pointer if the size is 0
		m_front = m_buffer;

	else
	{
		Uint32 offset = (Uint8*)m_front - (Uint8*)m_buffer;
		offset = (offset + max) % m_capacity;
		m_front = (Uint8*)m_buffer + offset;
	}

	// Callback
	if (m_onReadData)
		m_onReadData(buffer, max);

	// Return the amount of data that was read
	return max;
}


///////////////////////////////////////////////////////////
Uint32 BufferStream::write(void* data, Uint32 size)
{
	if (m_pipe)
	{
		// Call the read callback
		if (m_onReadData)
			m_onReadData(data, size);

		// Pipe data directly through
		m_pipe->write(data, size);
	}
	else
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

		// Callback
		if (m_onWriteData)
			m_onWriteData(data, size);
	}

	return size;
}


///////////////////////////////////////////////////////////
void BufferStream::pipe(WriteStream* stream)
{
	m_pipe = stream;

	if (m_size)
	{
		// Write all data currently in the buffer
		Uint32 roffset = m_capacity - ((Uint8*)m_front - (Uint8*)m_buffer);
		if (roffset < m_size)
		{
			stream->write(m_front, roffset);
			stream->write(m_buffer, m_size - roffset);

			if (m_onReadData)
			{
				m_onReadData(m_front, roffset);
				m_onReadData(m_buffer, m_size - roffset);
			}
		}
		else
		{
			stream->write(m_front, m_size);
			if (m_onReadData)
				m_onReadData(m_front, m_size);
		}

		// Reset properties
		m_size = 0;
		m_front = m_buffer;
	}
}


///////////////////////////////////////////////////////////
void BufferStream::unpipe()
{
	m_pipe = 0;
}


///////////////////////////////////////////////////////////
void BufferStream::flush()
{
	// Reset size and pointers
	m_size = 0;
	m_front = m_buffer;
}


///////////////////////////////////////////////////////////
Uint32 BufferStream::size() const
{
	return m_size;
}


///////////////////////////////////////////////////////////
Uint32 BufferStream::capacity() const
{
	return m_capacity;
}


}