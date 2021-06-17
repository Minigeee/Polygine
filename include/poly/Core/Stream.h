#ifndef POLY_STREAM_H
#define POLY_STREAM_H

#include <poly/Core/DataTypes.h>
#include <poly/Core/RingBuffer.h>

#include <functional>
#include <mutex>

namespace poly
{


class WriteStream;


///////////////////////////////////////////////////////////
/// \brief The base class for data stream that can be read from
/// \ingroup Core
///
///////////////////////////////////////////////////////////
class ReadStream
{
	friend WriteStream;

public:
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	///////////////////////////////////////////////////////////
	ReadStream();

	///////////////////////////////////////////////////////////
	/// \brief Virtual destructor
	///
	///////////////////////////////////////////////////////////
	virtual ~ReadStream();

#ifndef DOXYGEN_SKIP
	ReadStream(const ReadStream&) = delete;
	ReadStream& operator=(const ReadStream&) = delete;
	ReadStream(ReadStream&&);
	ReadStream& operator=(ReadStream&&);
#endif

	///////////////////////////////////////////////////////////
	/// \brief Read data from the read stream into an output buffer
	///
	/// \param buffer A pointer to the output buffer to read data into
	/// \param max The maximum number of bytes to read from the stream
	///
	/// \return The actual number of bytes that was read from the stream
	///
	///////////////////////////////////////////////////////////
	virtual Uint32 read(void* buffer, Uint32 max) = 0;

	///////////////////////////////////////////////////////////
	/// \brief Connect this read stream to an output write stream
	///
	/// This function connects this read stream to a write stream,
	/// but doesn't actuall read or write data from either streams.
	/// It keeps track of the write stream so that data can be pushed
	/// from this read stream into the output write stream, or so
	/// that the write stream can pull data from this read stream
	/// in the future.
	///
	/// The flow of data of streams that are piped to each other
	/// can be controlled by the push of the read stream, or by
	/// the pull of the write stream.
	///
	/// It is ok to move write and read streams after piping them,
	/// the move functions automatically update the internal pointers
	/// to point to the correct location.
	///
	/// \param output A pointer to an output stream
	///
	///////////////////////////////////////////////////////////
	void pipe(WriteStream* output);

	///////////////////////////////////////////////////////////
	/// \brief Remove the specified output write stream as a pipe connection
	///
	/// This removes the connection between streams.
	///
	/// \see pipe
	///
	/// \param output A pointer to the write stream to remove as a connection
	///
	///////////////////////////////////////////////////////////
	void unpipe(WriteStream* output);

protected:
	std::vector<WriteStream*> m_outputs;			//!< The list of output streams
};


///////////////////////////////////////////////////////////
/// \brief The base class for data streams that can be written to
/// \ingroup Core
///
///////////////////////////////////////////////////////////
class WriteStream
{
	friend ReadStream;

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	WriteStream();

	///////////////////////////////////////////////////////////
	/// \brief Virtual destructor
	///
	///////////////////////////////////////////////////////////
	virtual ~WriteStream();

#ifndef DOXYGEN_SKIP
	WriteStream(const WriteStream&) = delete;
	WriteStream& operator=(const WriteStream&) = delete;
	WriteStream(WriteStream&&);
	WriteStream& operator=(WriteStream&&);
#endif

	///////////////////////////////////////////////////////////
	/// \brief Write data into the stream
	///
	/// \param data A pointer to the data to write into the stream
	/// \param size The size of the data to write into the stream in bytes
	///
	/// \return The actual number of bytes written into the stream
	///
	///////////////////////////////////////////////////////////
	virtual Uint32 write(const void* data, Uint32 size) = 0;

protected:
	std::vector<ReadStream*> m_inputs;		//!< The list of input read streams
};


}

#endif