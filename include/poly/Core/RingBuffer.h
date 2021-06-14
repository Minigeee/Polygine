#ifndef POLY_RING_BUFFER_H
#define POLY_RING_BUFFER_H

#include <poly/Core/DataTypes.h>

namespace poly
{


///////////////////////////////////////////////////////////
class RingBuffer
{
public:
	RingBuffer();

	~RingBuffer();

	Uint32 read(void* buffer, Uint32 max);

	void write(void* data, Uint32 size);

	void clear();

	Uint32 size() const;

	Uint32 capacity() const;

protected:
	void* m_buffer;
	void* m_front;
	Uint32 m_size;
	Uint32 m_capacity;
};


}

#endif