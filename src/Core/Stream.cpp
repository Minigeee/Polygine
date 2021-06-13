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
void ReadStream::pipe(WriteStream* output)
{
	m_outputs.push_back(output);
	output->m_inputs.push_back(this);
}


///////////////////////////////////////////////////////////
void ReadStream::unpipe(WriteStream* output)
{
	std::vector<ReadStream*> inputs = output->m_inputs;
	for (auto it = inputs.begin(); it != inputs.end(); ++it)
	{
		if (*it == this)
		{
			inputs.erase(it);
			break;
		}
	}

	for (auto it = m_outputs.begin(); it != m_outputs.end(); ++it)
	{
		if (*it == output)
		{
			m_outputs.erase(it);
			break;
		}
	}
}


///////////////////////////////////////////////////////////
WriteStream::WriteStream()
{

}


///////////////////////////////////////////////////////////
WriteStream::~WriteStream()
{

}


///////////////////////////////////////////////////////////
BufferStream::BufferStream() :
	m_buffer		(0),
	m_front			(0),
	m_size			(0),
	m_capacity		(0)
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
	Uint32 numRead = 0;

	// Return null if the buffer doesn't exist or size is 0
	if (m_buffer && m_size)
	{
		numRead = max > m_size ? m_size : max;

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
	}

	for (Uint32 i = 0; i < m_inputs.size(); ++i)
	{
		if (numRead >= max)
			break;

		// If there is an input pipe, and there is still some data that needs to be read, read from input streams
		numRead += m_inputs[i]->read((Uint8*)buffer + numRead, max - numRead);
	}

	// Return the amount of data that was read
	return numRead;
}


///////////////////////////////////////////////////////////
Uint32 BufferStream::write(void* data, Uint32 size)
{
	if (m_outputs.size())
	{
		Uint32 numBytesWrote = 0;

		for (Uint32 i = 0; i < m_outputs.size(); ++i)
			numBytesWrote += m_outputs[i]->write(data, size);

		return numBytesWrote;
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

		return size;
	}
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