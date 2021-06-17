#ifndef POLY_RING_BUFFER_H
#define POLY_RING_BUFFER_H

#include <poly/Core/DataTypes.h>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief A buffer memory structure that uses a ring queue
///
///////////////////////////////////////////////////////////
class RingBuffer
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Nothing is allocated in the constructor
	///
	///////////////////////////////////////////////////////////
	RingBuffer();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// Frees any allocated memory
	///
	///////////////////////////////////////////////////////////
	~RingBuffer();

	///////////////////////////////////////////////////////////
	/// \brief Read data from the ring buffer
	///
	/// This reads data from the front of the ring buffer and copies
	/// the data into the given output buffer pointer. The maximum amount
	/// of buffer that is read from the buffer has to be specified
	/// with the \a max parameter so that overflow doesn't occur
	/// when copying data. This parameter should be given in number
	/// of bytes. The actual amount of data that was read from the
	/// buffer is returned from the function.
	///
	/// \param buffer The output buffer to copy data into
	/// \param max The maximum number of bytes to read from the buffer
	///
	/// \return The actual number of bytes that was read
	///
	///////////////////////////////////////////////////////////
	Uint32 read(void* buffer, Uint32 max);

	///////////////////////////////////////////////////////////
	/// \brief Write data into the buffer
	///
	/// Data is written into the back of the buffer, and when the
	/// buffer doesn't have enough space to write the new data into,
	/// the entire buffer is reallocated to be twice the new size
	/// of the buffer, and all old data is copied into the new memory
	/// space.
	///
	/// \param data A pointer to the data that should be written into the buffer
	/// \param size The number of bytes to write into the buffer
	///
	///////////////////////////////////////////////////////////
	void write(const void* data, Uint32 size);

	///////////////////////////////////////////////////////////
	/// \brief Clear the ring buffer
	///
	/// This completely resets the buffer, except for the underlying
	/// memory space. Destructors are not invoked.
	///
	///////////////////////////////////////////////////////////
	void clear();

	///////////////////////////////////////////////////////////
	/// \brief Get the number of bytes currently in use in the buffer
	///
	/// \return The number of bytes currently in use in the buffer
	///
	///////////////////////////////////////////////////////////
	Uint32 size() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the size of the underlying memory space in bytes
	///
	/// \return The size of the underlying memory space in bytes
	///
	///////////////////////////////////////////////////////////
	Uint32 capacity() const;

protected:
	void* m_buffer;			//!< A pointer to the beggining of the memory space
	void* m_front;			//!< A pointer to the beggining of the memory currently in use
	Uint32 m_size;			//!< The number of bytes currently in use
	Uint32 m_capacity;		//!< The size of the memory space in bytes
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::RingBuffer
/// \ingroup Core
///
/// The ring buffer is a simple memory structure that allocates
/// space for data in a way similar to a ring queue. When data
/// is written into the buffer, the data is copied onto the end
/// of the section where memory is currently in use. When data
/// is read from the buffer, data is returned starting from the
/// beggining of the section where memory is currently being used,
/// and the front pointer is advanced by however much data was
/// read. This method allows data to remain in the same section
/// upon removal, so that data doesn't ever have to be moved around
/// in memory.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Temporary buffer to read and write data
/// Uint8 data[100];
///
/// // Fill the data array
///
/// RingBuffer buffer;
///
/// // Write the first 50 bytes
/// buffer.write(data, 50);
///
/// // Read all data from the buffer
/// // The max parameter is set to 100 because that is the size of our array
/// Uint32 bytesRead = buffer.read(data, 100);
///
/// // bytesRead should now contain the value 50
///
/// \endcode
///
///////////////////////////////////////////////////////////