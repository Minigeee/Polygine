#ifndef POLY_STREAM_H
#define POLY_STREAM_H

#include <poly/Core/DataTypes.h>

#include <functional>
#include <mutex>

namespace poly
{


class WriteStream;


///////////////////////////////////////////////////////////
class ReadStream
{
	friend WriteStream;

public:
	ReadStream();

	virtual ~ReadStream();

#ifndef DOXYGEN_SKIP
	ReadStream(const ReadStream&) = delete;
	ReadStream& operator=(const ReadStream&) = delete;
	ReadStream(ReadStream&&);
	ReadStream& operator=(ReadStream&&);
#endif

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

#ifndef DOXYGEN_SKIP
	WriteStream(const WriteStream&) = delete;
	WriteStream& operator=(const WriteStream&) = delete;
	WriteStream(WriteStream&&);
	WriteStream& operator=(WriteStream&&);
#endif

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
	std::mutex m_mutex;
	void* m_buffer;
	void* m_front;
	Uint32 m_size;
	Uint32 m_capacity;
};


}

#endif