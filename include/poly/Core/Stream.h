#ifndef POLY_STREAM_H
#define POLY_STREAM_H

#include <poly/Core/DataTypes.h>

#include <functional>

namespace poly
{


class WriteStream;


///////////////////////////////////////////////////////////
class ReadStream
{
public:
	ReadStream();

	virtual ~ReadStream();

	virtual Uint32 read(void* buffer, Uint32 max) = 0;

	void pipe(WriteStream* output);

	void unpipe(WriteStream* output);

protected:
	std::vector<WriteStream*> m_outputs;
};


///////////////////////////////////////////////////////////
class WriteStream
{
	friend ReadStream;

public:
	WriteStream();

	virtual ~WriteStream();

	virtual Uint32 write(void* data, Uint32 size) = 0;

protected:
	std::vector<ReadStream*> m_inputs;
};


///////////////////////////////////////////////////////////
class BufferStream :
	public ReadStream,
	public WriteStream
{
public:
	BufferStream();

	virtual ~BufferStream();

	virtual Uint32 read(void* buffer, Uint32 max) override;

	virtual Uint32 write(void* data, Uint32 size) override;

	virtual void flush();

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