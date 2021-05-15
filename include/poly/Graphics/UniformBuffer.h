#ifndef POLY_UNIFORM_BLOCK_H
#define POLY_UNIFORM_BLOCK_H

#include <poly/Graphics/VertexBuffer.h>

#include <poly/Math/Vector2.h>
#include <poly/Math/Vector3.h>
#include <poly/Math/Vector4.h>
#include <poly/Math/Matrix2.h>
#include <poly/Math/Matrix3.h>
#include <poly/Math/Matrix4.h>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief A vertex buffer that holds uniform data for shaders
///
///////////////////////////////////////////////////////////
class UniformBuffer
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	UniformBuffer();

	///////////////////////////////////////////////////////////
	/// \brief Get the required alignment of uniform blocks in bytes
	///
	/// \return The required alignment value of uniform blocks in bytes
	///
	///////////////////////////////////////////////////////////
	static Uint32 getUniformBlockAlignment();

	///////////////////////////////////////////////////////////
	/// \brief Bind the uniform buffer to a specific bind point, for the purpose of accessing data in a shader
	///
	/// The purpose of this bind function is to bind the uniform
	/// buffer to a certain bind index, so that the buffer's data
	/// can be accessed from a uniform block in a shader. This function
	/// should not be used to push data into the buffer, instead
	/// the pushData() function should be used for that purpose.
	///
	/// When binding the uniform buffer, only a certain range will
	/// be bound. This range can be set by specifying the \a offset
	/// and the \a size parameters, which determine the offset of
	/// the range and the size of the range, in bytes. If \a offset
	/// is set to 0xFFFFFFFF, which it is by default, the uniform
	/// buffer will automatically use the offset of the previous
	/// batch of data that was pushed to the buffer. If size is set
	/// to 0, then the previous size will also be used.
	///
	/// This function is called by Shader::bindUniformBlock(), so
	/// in most cases, this function will not need to be called by
	/// the user.
	///
	/// \param index The bind point index to bind the uniform buffer to
	/// \param offset The offset of the range to bind in bytes
	/// \param size The size of the range to bind in bytes
	///
	///////////////////////////////////////////////////////////
	void bind(Uint32 index, Uint32 offset = 0xFFFFFFFFu, Uint32 size = 0);

	///////////////////////////////////////////////////////////
	/// \brief Create or resize the buffer with a certain size
	///
	/// Creating the buffer will wipe all previous data if it existed,
	/// and the buffer will created with the specified buffer usage flag.
	///
	/// \param size The size of the buffer in bytes
	/// \param usage The usage flag used to optimize the buffer based on its usage patterns
	///
	///////////////////////////////////////////////////////////
	void create(Uint32 size, BufferUsage usage = BufferUsage::Stream);

	///////////////////////////////////////////////////////////
	/// \brief Push data to the uniform buffer
	///
	/// This function will append the data to the end of the existing
	/// data, end when it runs out of buffer space, the data will
	/// be pushed to the start of the buffer again.
	///
	/// The data being pushed should be a uniform block struct
	/// for most efficient use. This function will return the
	/// offset of the data that was pushed, so that this section
	/// of data can be bound at a later time. The size and offset
	/// of the data are also recorded until the next time data
	/// is pushed to the buffer.
	///
	/// \param data The data to push to the buffer
	///
	/// \return The offset of the data that was pushed, in bytes
	///
	///////////////////////////////////////////////////////////
	template <typename T>
	Uint32 pushData(const T& data);

	///////////////////////////////////////////////////////////
	/// \brief Get the uniform buffer id
	///
	/// \return The OpenGL id
	///
	///////////////////////////////////////////////////////////
	Uint32 getId() const;

	///////////////////////////////////////////////////////////
	/// \brief Get the uniform buffer size in bytes
	///
	/// \return The buffer size in bytes
	///
	///////////////////////////////////////////////////////////
	Uint32 getSize() const;

private:
	VertexBuffer m_uniformBuffer;			//!< The internal vertex buffer
	Uint32 m_offset;						//!< The current offset of data in bytes
	Uint32 m_prevOffset;					//!< The offset of the previous batch of data
	Uint32 m_prevSize;						//!< The size of the previous batch of data

	static Uint32 s_uniformBlockAlignment;	//!< The uniform block alignment requirement
};


///////////////////////////////////////////////////////////
/// \brief A wrapper struct for uniform buffer data types
///
/// This will add the required padding for the std140 uniform
/// block layout rules, with the option of adding extra padding
/// at the end.
///
///////////////////////////////////////////////////////////
template <typename T, Uint32 N = 0>
struct UniformBufferType { };


#ifndef DOXYGEN_SKIP

///////////////////////////////////////////////////////////
template <Uint32 N>
struct UniformBufferType<bool, N>
{
	int& operator=(bool x) { m_data = x; return m_data; }

private:
	int m_data;
	float m_padding[N - 1];
};


///////////////////////////////////////////////////////////
template <>
struct UniformBufferType<bool, 0>
{
	int& operator=(bool x) { m_data = x; return m_data; }

private:
	int m_data;
};


///////////////////////////////////////////////////////////
template <Uint32 N>
struct UniformBufferType<int, N>
{
	int& operator=(int x) { m_data = x; return m_data; }

private:
	int m_data;
	float m_padding[N - 1];
};


///////////////////////////////////////////////////////////
template <>
struct UniformBufferType<int, 0>
{
	int& operator=(int x) { m_data = x; return m_data; }

private:
	int m_data;
};


///////////////////////////////////////////////////////////
template <Uint32 N>
struct UniformBufferType<float, N>
{
	float& operator=(float x) { m_data = x; return m_data; }

private:
	float m_data;
	float m_padding[N - 1];
};


///////////////////////////////////////////////////////////
template <>
struct UniformBufferType<float, 0>
{
	float& operator=(float x) { m_data = x; return m_data; }

private:
	float m_data;
};


///////////////////////////////////////////////////////////
template <Uint32 N>
struct UniformBufferType<Vector2f, N>
{
	Vector2f& operator=(Vector2f x) { m_data = x; return m_data; }

private:
	Vector2f m_data;
	float m_padding[N - 2];
};


///////////////////////////////////////////////////////////
template <>
struct UniformBufferType<Vector2f, 0>
{
	Vector2f& operator=(Vector2f x) { m_data = x; return m_data; }

private:
	Vector2f m_data;
};


///////////////////////////////////////////////////////////
template <Uint32 N>
struct UniformBufferType<Vector3f, N>
{
	Vector3f& operator=(Vector3f x) { m_data = x; return m_data; }

private:
	Vector3f m_data;
	float m_padding[N - 3];
};


///////////////////////////////////////////////////////////
template <>
struct UniformBufferType<Vector3f, 0>
{
	Vector3f& operator=(Vector3f x) { m_data = x; return m_data; }

private:
	Vector3f m_data;
};


///////////////////////////////////////////////////////////
template <Uint32 N>
struct UniformBufferType<Vector4f, N>
{
	Vector4f& operator=(Vector4f x) { m_data = x; return m_data; }

private:
	Vector4f m_data;
	float m_padding[N - 4];
};


///////////////////////////////////////////////////////////
template <>
struct UniformBufferType<Vector4f, 0>
{
	Vector4f& operator=(Vector4f x) { m_data = x; return m_data; }

private:
	Vector4f m_data;
};


///////////////////////////////////////////////////////////
template <Uint32 N>
struct UniformBufferType<Matrix2f, N>
{
	UniformBufferType<Matrix2f, N>& operator=(Matrix2f a) { x = a.x; y = a.y; return *this; }

private:
	Vector2f x;
	Vector2f m_p1;
	Vector2f y;
	float m_padding[N - 6];
};


///////////////////////////////////////////////////////////
template <>
struct UniformBufferType<Matrix2f, 0>
{
	UniformBufferType<Matrix2f, 0>& operator=(Matrix2f a) { x = a.x; y = a.y; return *this; }

private:
	Vector2f x;
	Vector2f m_p1;
	Vector2f y;
	Vector2f m_p2;
};


///////////////////////////////////////////////////////////
template <Uint32 N>
struct UniformBufferType<Matrix3f, N>
{
	UniformBufferType<Matrix3f, N>& operator=(Matrix3f a) { x = a.x; y = a.y; z = a.z; return *this; }

private:
	Vector3f x;
	float m_p1;
	Vector3f y;
	float m_p2;
	Vector3f z;
	float m_padding[N - 11];
};


///////////////////////////////////////////////////////////
template <>
struct UniformBufferType<Matrix3f, 0>
{
	UniformBufferType<Matrix3f, 0>& operator=(Matrix3f a) { x = a.x; y = a.y; z = a.z; return *this; }

private:
	Vector3f x;
	float m_p1;
	Vector3f y;
	float m_p2;
	Vector3f z;
	float m_p3;
};


///////////////////////////////////////////////////////////
template <Uint32 N>
struct UniformBufferType<Matrix4f, N>
{
	UniformBufferType<Matrix4f, N>& operator=(Matrix4f a) { m = a; return *this; }

private:
	Matrix4f m;
	float m_padding[N - 16];
};


///////////////////////////////////////////////////////////
template <>
struct UniformBufferType<Matrix4f, 0>
{
	UniformBufferType<Matrix4f, 0>& operator=(Matrix4f a) { m = a; return *this; }

private:
	Matrix4f m;
};

#endif

}

#include <poly/Graphics/UniformBuffer.inl>

#endif


///////////////////////////////////////////////////////////
/// \class poly::UniformBuffer
/// \ingroup Graphics
///
/// The uniform buffer class is used to store uniform data to
/// be used with uniform blocks in shaders. The purpose of using
/// a uniform buffer over simply setting the uniform values in
/// a shader is to reduce the amount of data that needs to be
/// passed to the GPU while rendering. This means that a uniform
/// buffer should be used for uniforms that don't change value
/// too many times within the same render frame, such as camera
/// values, lighting uniforms, etc. It is less effective for
/// uniforms that change value often within the same frame, such
/// as material uniforms.
///
/// To use a uniform buffer, the buffer must first be created
/// with a certain size in bytes using create(). The size of
/// the buffer can be equal to the uniform block that is being
/// pushed to the buffer, but it can be several times larger as
/// well. Data is pushed to the end of the existing data in a
/// way similar to a ring queue. Whenever the data being pushed
/// reaches the end of the buffer, the data offset is reset to
/// the front of the buffer, and data is continued to be pushed
/// starting at the beggining again.
///
/// After creating the buffer, data can be pushed to the buffer
/// with pushData(). The function accepts a single data object
/// and while this can be any object, including an integer,
/// pushing an entire uniform block struct would be easiest.
/// This function returns the byte offset of the data that was
/// pushed, and this offset can then be used to bind the buffer
/// range at a later time. To bind the uniform buffer to a uniform
/// block in a shader, use Shader::bindUniformBlock() with the
/// current buffer. That function takes optional \a offset and
/// \a size parameters to specify the range that should be bound,
/// but if the are not specified, the offset and size of the
/// previous data batch are used.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Basic uniform block struct
/// struct UniformBlock_Camera
/// {
///		// Using the UniformBufferType struct will add the correct padding
///		// Format:
///		// UniformBufferType<type, size>
///		// Where the size of the type can be specified in bytes, or it can
///		// be left empty to use the default size
///
///		UniformBufferType<Matrix4f> m_projView;
///		UniformBufferType<Vector3f> m_cameraPos;
///		UniformBufferType<float> m_nearPlane;
///		UniformBufferType<float> m_farPlane;
/// };
///
/// // Create the uniform buffer
/// UniformBuffer buffer;
///
/// // Make the buffer 12x bigger than usual so that
/// // unsynchronized data updates can be used
/// buffer.create(sizeof(UniformBlock_Camera) * 12);
///
/// // Create a uniform block
/// UniformBlock_Camera block;
///
/// // Fill it with important info...
///
/// // Push the data
/// buffer.pushData(block);
///
/// // Other setup stuff, load shaders...
///
/// ...
///
/// // Bind it to a shader uniform block
/// shader.bindUniformBlock("Camera", buffer);
///
/// \endcode
///
///////////////////////////////////////////////////////////