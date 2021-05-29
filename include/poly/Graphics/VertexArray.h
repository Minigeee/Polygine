#ifndef POLY_VERTEX_ARRAY_H
#define POLY_VERTEX_ARRAY_H

#include <poly/Core/DataTypes.h>

#include <poly/Graphics/VertexBuffer.h>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief The draw mode that determines how the vertex order should be interpreted
///
///////////////////////////////////////////////////////////
enum class DrawMode
{
	Points = 0,
	Lines,
	LineLoop,
	LineStrip,
	Triangles,
	TriangleStrip,
	TriangleFan
};

///////////////////////////////////////////////////////////
/// \brief Stores and maps an array of vertex buffers for rendering
///
///////////////////////////////////////////////////////////
class VertexArray
{
public:

#ifndef DOXYGEN_SKIP
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;
	VertexArray(VertexArray&&) = default;
	VertexArray& operator=(VertexArray&&) = default;
#endif

	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// This does not create the internal vertex array object
	///
	///////////////////////////////////////////////////////////
	VertexArray();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// This will destroy the internal vertex array object, if it exists
	///
	///////////////////////////////////////////////////////////
	~VertexArray();

	///////////////////////////////////////////////////////////
	/// \brief Bind the vertex array to perform operations on it
	///
	/// Binding the vertex array is necessary to perform any operations
	/// using it. This function will not be necessary to call in most
	/// cases because all other available vertex array operations
	/// will ensure that the vertex array is bound before proceeding.
	///
	///////////////////////////////////////////////////////////
	void bind();

	///////////////////////////////////////////////////////////
	/// \brief Unbind the current vertex array if it is the current bound
	///
	/// If this vertex array is not bound, nothing happens.
	///
	///////////////////////////////////////////////////////////
	void unbind();

	///////////////////////////////////////////////////////////
	/// \brief Add a vertex buffer and additional metadata
	///
	/// Add a vertex buffer to the specified index in the vertex array.
	/// For all added buffers, the size of each element must be
	/// specified in number of primitive data types used to define each
	/// element. For example, if each element is a Vector4f, size
	/// should be 4 because each element is made of 4 floats.
	/// Similarly, Vector2i has a size of 2 because it is made
	/// of 2 integers.
	///
	/// If interleaved data is stored in the vertex buffer, the
	/// stride and offset must also be specified. Stride is the size
	/// of all interleaved data per element combined, in bytes, and offset
	/// is the offset of the target data in each element, also in bytes.
	/// If interleaved data is not being used, both stride and offset
	/// can be ignored.
	///
	/// It is also possible to define instance data rather than
	/// vertex data adding a divisor parameter. The divisor determines
	/// how often instance data index is incremented. For example,
	/// if \a divisor is 1, the instance data will change every instance.
	/// If it is 2, the instance data will change every other instance
	/// that is rendered. This option should only be used if instanced
	/// rendering is being used.
	///
	/// \param buffer The vertex buffer to add to the array
	/// \param index The index to assign the buffer
	/// \param size The number of primitives each element is made of (1 - 4)
	/// \param stride The size of each element in bytes (only for interleaved data)
	/// \param offset The offset of the targeted data in each element in bytes (only for interleaved data)
	/// \param divisor The number of instances to render before changing the current instance data
	/// \param dtype The data type used to override the vertex buffer data type
	///
	///////////////////////////////////////////////////////////
	void addBuffer(VertexBuffer& buffer, Uint32 index, Uint32 size, Uint32 stride = 0, Uint32 offset = 0, Uint32 divisor = 0, GLType dtype = GLType::Unknown);

	///////////////////////////////////////////////////////////
	/// \brief Render the contents of the vertex array
	///
	/// The contents of the buffers stored in the vertex array
	/// are used to render. The data is rendered using the DrawMode
	/// specified using setDrawMode(), which is DrawMode::Triangles
	/// by default.
	///
	/// It is possible to specify the vertex range to rendering.
	/// By default, the vertex array will used the offset and number
	/// of vertices stored internally, set by setVertexOffset() and
	/// setNumVertices(), but the \a offset and \a vertices parameter
	/// will override the internal value if their values are set to
	/// anything other than 0xFFFFFFFF.
	///
	/// \param instances The number of instances to draw
	/// \param offset The offset to start drawing at, in number of vertices
	///
	///////////////////////////////////////////////////////////
	void draw(Uint32 instances = 1, Uint32 offset = 0xFFFFFFFF, Uint32 vertices = 0xFFFFFFFF);

	///////////////////////////////////////////////////////////
	/// \brief Set the number of vertices
	///
	/// \param numVertices Set the number of vertices
	///
	///////////////////////////////////////////////////////////
	void setNumVertices(Uint32 numVertices);

	///////////////////////////////////////////////////////////
	/// \brief Set the vertex offset (in number of vertices)
	///
	/// Set the vertex offset that should be used during rendering
	/// of the vertex offset.
	///
	/// \param offset The vertex offset
	///
	///////////////////////////////////////////////////////////
	void setVertexOffset(Uint32 offset);

	///////////////////////////////////////////////////////////
	/// \brief Set the draw mode
	///
	/// \param mode The draw mode
	///
	///////////////////////////////////////////////////////////
	void setDrawMode(DrawMode mode);

	///////////////////////////////////////////////////////////
	/// \brief Set the element buffer that should be used to render the vertex array
	///
	/// The element buffer is used when smooth shading is desired.
	/// The given buffer must already have been created and its data
	/// should already be pushed to the GPU.
	///
	/// \note The vertex array is unbound immediately after the element buffer is set
	/// \note The element buffer will be bound, but won't be unbound, so keep this in mind for future OpenGL operations
	///
	/// \param buffer The element buffer
	///
	///////////////////////////////////////////////////////////
	void setElementBuffer(VertexBuffer& buffer);

	///////////////////////////////////////////////////////////
	/// \brief Get the internal vertex array id
	///
	/// The id is the same as the OpenGL id
	///
	/// \return The internal OpenGL id
	///
	///////////////////////////////////////////////////////////
	Uint32 getId() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the number of vertices defined by the buffers stored in the array
	///
	/// \return The number of vertices in the array
	///
	///////////////////////////////////////////////////////////
	Uint32 getNumVertices() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the vertex offset that is used during rendering (in number of vertices)
	///
	/// \return The vertex offset used during rendering
	///
	///////////////////////////////////////////////////////////
	Uint32 getVertexOffset() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the current draw mode
	///
	/// \return The current draw mode
	///
	///////////////////////////////////////////////////////////
	DrawMode getDrawMode() const;

	///////////////////////////////////////////////////////////
	/// \brief Check if the vertex array contains an element vertex buffer
	///
	/// \return True if the vertex array contains an element vertex buffer
	///
	///////////////////////////////////////////////////////////
	bool hasElementBuffer() const;

private:
	Uint32 m_id;				//!< OpenGL id
	Uint32 m_numVertices;		//!< The number of vertices
	Uint32 m_vertexOffset;		//!< The vertex offset to use when rendering
	DrawMode m_drawMode;		//!< The current draw mode
	Uint32 m_elementBuffer;		//!< True if an element vertex buffer was added

	static Uint32 s_currentBound;
};

}

#endif

///////////////////////////////////////////////////////////
/// \class poly::VertexArray
/// \ingroup Graphics
///
/// Vertex arrays are a way to group vertex buffers together to
/// render verted data. Vertex arrays can hold vertex buffers
/// that contain data that are all adjacent to each other, and
/// it can also hold buffers that contain interleaved data.
/// Each buffer is assigned to an index, and additional metadata
/// can be specified so that shader programs know how to interperet
/// the buffer data. The index each buffer is assigned to can be
/// used in shaders to access the stored buffer data.
///
/// To draw a vertex array, set the draw mode (or just leave it
/// as DrawMode::Triangles) and call draw().
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Create a vertex array where each element is the same type
/// std::vector<Vector3f> verticesA(12);
///
/// // Add the vertex data to a vertex buffer
/// VertexBuffer bufferA;
/// bufferA.create(verticesA);
///
/// // Create and draw the vertex array
/// // The size of each element is 3 because Vector3f is being used
/// VertexArray a;
/// a.addBuffer(bufferA, 0, 3);
/// a.draw();
///
///
///
/// // Create a vertex array with interleaved data
/// struct Vertex
/// {
///		Vector3f m_position;
///		Vector3f m_normal;
///		Vector4f m_color;
/// }
///
/// std::vector<Vertex> verticesB(12);
///
/// // Add the vertex data to a vertex buffer
/// VertexBuffer bufferB;
/// bufferB.create(verticesB);
///
/// // Create and draw the vertex array
/// // With interleaved data, a new buffer for each attribute in the element is required,
/// // as well as specifying the stride and offset of each attribute
/// VertexArray b;
/// b.addBuffer(bufferB, 0, 3, sizeof(Vertex), 0 * sizeof(Vector3f));	// Position
/// b.addBuffer(bufferB, 1, 3, sizeof(Vertex), 1 * sizeof(Vector3f));	// Normal
/// b.addBuffer(bufferB, 2, 4, sizeof(Vertex), 2 * sizeof(Vector3f));	// Color
/// b.draw();
///
/// \endcode
///
///////////////////////////////////////////////////////////