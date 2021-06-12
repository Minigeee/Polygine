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

	virtual void pipe(WriteStream* stream) = 0;

	virtual void unpipe() = 0;

	void onReadData(const std::function<void(void*, Uint32)>& func);

protected:
	std::function<void(void*, Uint32)> m_onReadData;
};


///////////////////////////////////////////////////////////
class WriteStream
{
public:
	virtual ~WriteStream();

	virtual Uint32 write(void* data, Uint32 size) = 0;

	void onWriteData(const std::function<void(void*, Uint32)>& func);

protected:
	std::function<void(void*, Uint32)> m_onWriteData;
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

	virtual void pipe(WriteStream* stream) override;

	virtual void unpipe() override;

	virtual void flush();

	Uint32 size() const;

	Uint32 capacity() const;

protected:
	void* m_buffer;
	void* m_front;
	Uint32 m_size;
	Uint32 m_capacity;
	WriteStream* m_pipe;
};


}

#endif