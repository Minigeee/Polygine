#ifndef POLY_VERTEX_BUFFER_H
#define POLY_VERTEX_BUFFER_H

#include <poly/Core/DataTypes.h>

#include <poly/Graphics/GLType.h>

#include <vector>

namespace poly
{

class VertexArray;


///////////////////////////////////////////////////////////
/// \brief Targets to bind vertex buffers to
///
///////////////////////////////////////////////////////////
enum class BufferTarget
{
	Array = 0,			//!< This will be the standard target used in most cases, it stores regular vertex data (GL_ARRAY_BUFFER)
	Element,			//!< This can be used to store vertex index order (GL_ELEMENT_ARRAY_BUFFER)
	TransformFeedback,	//!< This is used to store transform feedback data (GL_TRANSFORM_FEEDBACK_BUFFER)
	Uniform				//!< This is used to store uniform block data (GL_UNIFORM_BUFFER)
};


///////////////////////////////////////////////////////////
/// \brief The main usage case of the target vertex buffer
///
///////////////////////////////////////////////////////////
enum class BufferUsage
{
	Static	= 0x88E4,	//!< The vertex buffer data will rarely ever change after initially setting the data
	Dynamic = 0x88E8,	//!< The vertex buffer data is expected to change pretty often
	Stream	= 0x88E0	//!< The vertex buffer data is expected to update every frame or every few frames
};


///////////////////////////////////////////////////////////
/// \brief An enum defining flags used when mapping a buffer
///
///////////////////////////////////////////////////////////
enum class MapBufferFlags
{
	Write				= 0x0002,	//!< Map the buffer for writing data
	Read				= 0x0001,	//!< Map the buffer for reading data
	Unsynchronized		= 0x0020,	//!< Map the buffer in an unsynchronized manner, meaning that the mapped data may still be in use (unsafe)
	InvalidateBuffer	= 0x0008,	//!< Indicate that any data stored in the buffer before mapping may be discarded
	InvalidateRange		= 0x0004	//!< Indicate that any data stored in the specified range before mapping may be discarded
};

///////////////////////////////////////////////////////////
/// \brief The binary AND operator for map buffer flags
///
/// \param a The first operand
/// \param b The second operand
///
/// \return The result
///
///////////////////////////////////////////////////////////
MapBufferFlags operator&(MapBufferFlags a, MapBufferFlags b);

///////////////////////////////////////////////////////////
/// \brief The binary OR operator for map buffer flags
///
/// \param a The first operand
/// \param b The second operand
///
/// \return The result
///
///////////////////////////////////////////////////////////
MapBufferFlags operator|(MapBufferFlags a, MapBufferFlags b);

///////////////////////////////////////////////////////////
/// \brief The binary NOT operator for map buffer flags
///
/// \param a The operand
///
/// \return The result
///
///////////////////////////////////////////////////////////
MapBufferFlags operator~(MapBufferFlags a);

///////////////////////////////////////////////////////////
/// \brief The binary AND assignment operator for map buffer flags
///
/// \param a The first operand (the one that is being assigned to)
/// \param b The second operand
///
/// \return The result
///
///////////////////////////////////////////////////////////
MapBufferFlags& operator&=(MapBufferFlags& a, MapBufferFlags b);

///////////////////////////////////////////////////////////
/// \brief The binary OR assignment operator for map buffer flags
///
/// \param a The first operand (the one that is being assigned to)
/// \param b The second operand
///
/// \return The result
///
///////////////////////////////////////////////////////////
MapBufferFlags& operator|=(MapBufferFlags& a, MapBufferFlags b);


///////////////////////////////////////////////////////////
/// \brief A class that stores and manages vertex data on the GPU
///
///////////////////////////////////////////////////////////
class VertexBuffer
{
	friend VertexArray;

public:

#ifndef DOXYGEN_SKIP
	VertexBuffer(const VertexBuffer& other) = delete;
	VertexBuffer& operator=(const VertexBuffer& other) = delete;
	VertexBuffer(VertexBuffer&& other) = default;
	VertexBuffer& operator=(VertexBuffer && other) = default;
#endif

	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// This will not create a new vertex buffer object
	///
	///////////////////////////////////////////////////////////
	VertexBuffer();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// This will destroy the internal vertex buffer object, if it exists
	///
	///////////////////////////////////////////////////////////
	~VertexBuffer();

	///////////////////////////////////////////////////////////
	/// \brief Bind the vertex buffer to the last used target
	///
	/// \note Calling this will create a new vertex buffer object if it doesn't exist
	///
	///////////////////////////////////////////////////////////
	void bind();

	///////////////////////////////////////////////////////////
	/// \brief Unbind the current vertex buffer if it is the current bound
	///
	/// If this vertex array is not bound, nothing happens. This
	/// may not work when the buffer is bound to multiple targets
	/// at once. When that is the case, the buffer is only unbound
	/// from the last target it was bound to.
	///
	///////////////////////////////////////////////////////////
	void unbind();

	///////////////////////////////////////////////////////////
	/// \brief Bind the vertex buffer to a target
	///
	/// \param target The target to bind to
	///
	/// \note Calling this will create a new vertex buffer object if it doesn't exist
	///
	///////////////////////////////////////////////////////////
	void bind(BufferTarget target);

	///////////////////////////////////////////////////////////
	/// \brief Bind the vertex buffer to a target that has multiple bind points
	///
	/// The only target that have multiple bind points are
	/// BufferTarget::TransformFeedback and BufferTarget::Uniform.
	/// This function also provides the option to bind a specific
	/// range of the buffer to the specified bind index.
	///
	/// \param target The target to bind to
	/// \param index The bind point index
	/// \param offset The offset of the range to bind in bytes
	/// \param size The size of the range to bind in bytes
	///
	/// \note Calling this will create a new vertex buffer object if it doesn't exist
	///
	///////////////////////////////////////////////////////////
	void bind(BufferTarget target, Uint32 index, Uint32 offset = 0, Uint32 size = 0);

	///////////////////////////////////////////////////////////
	/// \brief Create a vertex buffer from data
	///
	/// The data type being used will automatically be inferred from the data if it
	/// is a numerical primitive type (ints and floats), and it will default
	/// to GLType::Float if a custom struct is used. It is possible
	/// to override the inferred data type by giving the parameter
	/// \a dtype a value other than GLType::Unknown. The buffer must be
	/// bound to the desired BufferTarget before being created.
	/// If the buffer isn't bound before being created, BufferTarget::Array
	/// will be used by default
	///
	/// \param data A list of data to store in vertex buffer on GPU
	/// \param usage How the data will be modified
	/// \param dtype The type of data being stored
	///
	///////////////////////////////////////////////////////////
	template <typename T>
	void create(const std::vector<T>& data, BufferUsage usage = BufferUsage::Static, GLType dtype = GLType::Unknown);

	///////////////////////////////////////////////////////////
	/// \brief Create a vertex buffer from data
	///
	/// The data type being used will automatically be inferred from the data if it
	/// is a numerical primitive type (ints and floats), and it will default
	/// to GLType::Float if a custom struct is used. It is possible
	/// to override the inferred data type by giving the parameter
	/// \a dtype a value other than GLType::Unknown. The buffer must be
	/// bound to the desired BufferTarget before being created.
	/// If the buffer isn't bound before being created, BufferTarget::Array
	/// will be used by default
	///
	/// \param data A list of data to store in vertex buffer on GPU
	/// \param num The number of elements to create the buffer with
	/// \param usage How the data will be modified
	/// \param dtype The type of data being stored
	///
	///////////////////////////////////////////////////////////
	template <typename T>
	void create(const T* data, Uint32 num, BufferUsage usage = BufferUsage::Static, GLType dtype = GLType::Unknown);

	///////////////////////////////////////////////////////////
	/// \brief Update data in a vertex buffer
	///
	/// The vertex buffer must have been created and its size
	/// must be bigger than the new data size
	///
	/// \param data The new data to replace in the vertex buffer
	/// \param offset The destination offset to copy the data to (measured in number of elements)
	///
	///////////////////////////////////////////////////////////
	template <typename T>
	void update(const std::vector<T>& data, Uint32 offset = 0);

	///////////////////////////////////////////////////////////
	/// \brief Update data in a vertex buffer
	///
	/// The vertex buffer must have been created and its size
	/// must be bigger than the new data size
	///
	/// \param data The new data to replace in the vertex buffer
	/// \param num The number of elements to update
	/// \param offset The destination offset to copy the data to (measured in number of elements)
	///
	///////////////////////////////////////////////////////////
	template <typename T>
	void update(const T* data, Uint32 num, Uint32 offset = 0);

	///////////////////////////////////////////////////////////
	/// \brief Update data in a vertex buffer
	///
	/// The vertex buffer must have been created and its size
	/// must be bigger than the new vertex buffer's size
	///
	/// \param data The new vertex buffer to copy the data from
	/// \param offset The destination offset to copy the data to (measured in number of bytes)
	///
	///////////////////////////////////////////////////////////
	void update(const VertexBuffer& buffer, Uint32 offset = 0);

	///////////////////////////////////////////////////////////
	/// \brief Map data from the specified range to an address in memory
	///
	/// This function uses glMapBufferRange() and returns an address
	/// to the mapped data. Map flags may be used to determine what
	/// occurs before mapping the data.
	///
	/// \param offset The offset of the range to map in bytes
	/// \param size The size of the range to map in bytes
	/// \param flags The set of flags to pass to the mapping function
	///
	/// \return A pointer to the mapped data
	///
	///////////////////////////////////////////////////////////
	void* map(Uint32 offset, Uint32 size, MapBufferFlags flags);

	///////////////////////////////////////////////////////////
	/// \brief Unmap all previously mapped ranges
	///
	///////////////////////////////////////////////////////////
	void unmap();

	///////////////////////////////////////////////////////////
	/// \brief Set the vertex buffer's bind target without binding
	///
	/// \param target The target to bind to
	///
	///////////////////////////////////////////////////////////
	void setTarget(BufferTarget target);

	///////////////////////////////////////////////////////////
	/// \brief Get the vertex buffer id
	///
	/// \return The OpenGL id
	///
	///////////////////////////////////////////////////////////
	Uint32 getId() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the vertex buffer size in bytes
	///
	/// \return The buffer size in bytes
	///
	///////////////////////////////////////////////////////////
	Uint32 getSize() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the vertex buffer usage mode
	///
	/// \return A BufferUsage enum containing the update pattern (usage)
	///
	///////////////////////////////////////////////////////////
	BufferUsage getUsage() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the current or last used bind target
	///
	/// \return A BufferTarget enum containing the last used bind target
	///
	///////////////////////////////////////////////////////////
	BufferTarget getTarget() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the buffer data type
	///
	/// \return The data type being used by the vertex buffer
	///
	///////////////////////////////////////////////////////////
	GLType getDataType() const;

private:
	template <typename T>
	void setDataType();

	void bufferData(const void* data, Uint32 size, BufferUsage usage);

	void bufferSubData(const void* data, Uint32 size, Uint32 offset);

private:
	Uint32 m_id;				//!< The buffer id
	Uint32 m_size;				//!< The size of the buffer in bytes
	BufferUsage m_usage;		//!< The usage of the buffer
	BufferTarget m_target;		//!< The last bind target
	GLType m_type;				//!< The buffer data type

	static Uint32 s_currentBound[4];
};

}

#include <poly/Graphics/VertexBuffer.inl>

#endif

///////////////////////////////////////////////////////////
/// \class poly::VertexBuffer
/// \ingroup Graphics
///
/// VertexBuffer is a class that stores data on the GPU.
/// Each buffer must be bound to a certain BufferTarget using bind()
/// in order for any buffer operations to be done to it, including
/// creating the initial data. If create() is called without
/// calling bind(), BufferTarget::Array will be used by default.
/// When creating the initial buffer data, a BufferUsage must be
/// specified as well. BufferUsage::Static will be the most commonly
/// used type, which optimizes the buffer for static data.
/// BufferUsage::Dynamic optimizes the buffer for data that
/// changes relatively often, and BufferUsage::Stream should
/// be used when data in the buffer is changing every frame or
/// every few frames.
///
/// The most common use for VertexBuffer is for storing vertex
/// data for rendering.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// std::vector<float> vertices =
///	{
///		-0.5f,  0.5f, 0.0f,
///		-0.5f, -0.5f, 0.0f,
///		 0.5f,  0.5f, 0.0f,
///
///		-0.5f, -0.5f, 0.0f,
///		 0.5f, -0.5f, 0.0f,
///		 0.5f,  0.5f, 0.0f,
/// };
///
/// // Store vertices on GPU
/// VertexBuffer a;
/// a.create(vertices);
///
/// // It's also possible to create an empty buffer
/// VertexBuffer empty;
/// empty.create((float*)0, 100);
///
/// // A custom data structure
/// struct Data
/// {
///		int a, b;
/// };
/// 
/// std::vector<Data> data(10);
///
/// // Create a dynamic buffer using the uniform target
/// // We also need to specify that integer data is being used
/// VertexBuffer b;
/// b.bind(BufferTarget::Uniform);
/// b.create(data, BufferUsage::Dynamic, GLType::Int32);
///
/// \endcode
///
///////////////////////////////////////////////////////////