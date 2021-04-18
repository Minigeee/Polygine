#include <poly/Math/Vector2.h>
#include <poly/Math/Vector3.h>
#include <poly/Math/Vector4.h>
#include <poly/Math/Matrix2.h>
#include <poly/Math/Matrix3.h>
#include <poly/Math/Matrix4.h>

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
template <typename T>
inline Uint32 copyUniformBlockData(Uint8* dst, const T& data)
{
	// Standard implementation
	*(T*)dst = data;
	return sizeof(T);
}


///////////////////////////////////////////////////////////
template <>
inline Uint32 copyUniformBlockData(Uint8* dst, const bool& data)
{
	// Standard implementation
	*(bool*)dst = data;
	return sizeof(float);
}


///////////////////////////////////////////////////////////
template <typename T>
inline Uint32 copyUniformBlockData(Uint8* dst, const Vector2<T>& data)
{
	*(T*)(dst + 0) = data.x;
	*(T*)(dst + 4) = data.y;
	return 2 * sizeof(float);
}


///////////////////////////////////////////////////////////
template <typename T>
inline Uint32 copyUniformBlockData(Uint8* dst, const Vector3<T>& data)
{
	*(T*)(dst + 0) = data.x;
	*(T*)(dst + 4) = data.y;
	*(T*)(dst + 8) = data.z;
	return 4 * sizeof(float);
}


///////////////////////////////////////////////////////////
template <typename T>
inline Uint32 copyUniformBlockData(Uint8* dst, const Vector4<T>& data)
{
	*(T*)(dst + 0) = data.x;
	*(T*)(dst + 4) = data.y;
	*(T*)(dst + 8) = data.z;
	*(T*)(dst + 12) = data.w;
	return 4 * sizeof(float);
}


///////////////////////////////////////////////////////////
template <typename T>
inline Uint32 copyUniformBlockData(Uint8* dst, const Matrix2<T>& data)
{
#ifdef USE_COLUMN_MAJOR
	copyUniformBlockData(dst + 0, data.x);
	copyUniformBlockData(dst + 16, data.y);
#else
	Matrix2<T> m = transpose(data);
	copyUniformBlockData(dst + 0, m.x);
	copyUniformBlockData(dst + 16, m.y);
#endif

	return 8 * sizeof(float);
}


///////////////////////////////////////////////////////////
template <typename T>
inline Uint32 copyUniformBlockData(Uint8* dst, const Matrix3<T>& data)
{
#ifdef USE_COLUMN_MAJOR
	copyUniformBlockData(dst + 0, data.x);
	copyUniformBlockData(dst + 16, data.y);
	copyUniformBlockData(dst + 32, data.z);
#else
	Matrix3<T> m = transpose(data);
	copyUniformBlockData(dst + 0, m.x);
	copyUniformBlockData(dst + 16, m.y);
	copyUniformBlockData(dst + 32, m.z);
#endif

	return 12 * sizeof(float);
}


///////////////////////////////////////////////////////////
template <typename T>
inline Uint32 copyUniformBlockData(Uint8* dst, const Matrix4<T>& data)
{
#ifdef USE_COLUMN_MAJOR
	copyUniformBlockData(dst + 0, data.x);
	copyUniformBlockData(dst + 16, data.y);
	copyUniformBlockData(dst + 32, data.z);
	copyUniformBlockData(dst + 48, data.w);
#else
	Matrix4<T> m = transpose(data);
	copyUniformBlockData(dst + 0, m.x);
	copyUniformBlockData(dst + 16, m.y);
	copyUniformBlockData(dst + 32, m.z);
	copyUniformBlockData(dst + 48, m.w);
#endif

	return 16 * sizeof(float);
}


///////////////////////////////////////////////////////////
template <typename T>
struct UniformBlockPadding
{
	static Uint32 get()
	{
		Uint32 padding = sizeof(T);

		// General padding rules
		if (padding > 16)
			padding = 16;
		else if (padding < 4)
			padding = 4;

		return padding;
	}
};


///////////////////////////////////////////////////////////
template <typename T>
struct UniformBlockPadding<Vector2<T>>
{
	static Uint32 get()
	{
		return 2 * sizeof(float);
	}
};


///////////////////////////////////////////////////////////
template <typename T>
struct UniformBlockPadding<Vector3<T>>
{
	static Uint32 get()
	{
		return 4 * sizeof(float);
	}
};


///////////////////////////////////////////////////////////
template <typename T>
struct UniformBlockPadding<Vector4<T>>
{
	static Uint32 get()
	{
		return 4 * sizeof(float);
	}
};


///////////////////////////////////////////////////////////
template <typename T>
struct UniformBlockPadding<Matrix2<T>>
{
	static Uint32 get()
	{
		return 4 * sizeof(float);
	}
};


///////////////////////////////////////////////////////////
template <typename T>
struct UniformBlockPadding<Matrix3<T>>
{
	static Uint32 get()
	{
		return 4 * sizeof(float);
	}
};


///////////////////////////////////////////////////////////
template <typename T>
struct UniformBlockPadding<Matrix4<T>>
{
	static Uint32 get()
	{
		return 4 * sizeof(float);
	}
};


}


///////////////////////////////////////////////////////////
template <typename T>
inline void UniformBlock::addData(const T& data, Uint32 padding)
{
	// If the buffer has not been created, then initialize it with 256 bytes
	if (!m_vertexBuffer.getSize())
		setBufferSize(256);

	// Get padding
	if (!padding)
		padding = priv::UniformBlockPadding<T>::get();

	// Adjust current block size to fit padding
	m_blockSize = (m_blockSize + padding - 1) / padding * padding;

	// Use a size estimate to check if the buffer needs to be reset
	if (m_offset + m_blockSize + (Uint32)(sizeof(T) * 1.35f) > m_vertexBuffer.getSize())
	{
		// If the buffer offset is zero, then the buffer needs to be expanded
		if (m_offset == 0)
		{
			// Copy everything to a temporary buffer
			void* temp = malloc(m_blockSize);
			memcpy(temp, m_data + m_offset, m_blockSize);

			// Expand by x5
			setBufferSize(m_vertexBuffer.getSize() * 5);

			// Copy everything back into the main buffer
			memcpy(m_data, temp, m_blockSize);
		}

		// Otherwise, copy everything to the beggining
		else
			memcpy(m_data, m_data + m_offset, m_blockSize);

		m_offset = 0;
		m_bufferReset = true;
	}

	// Copy data to data buffer, and get exact size
	Uint32 size = priv::copyUniformBlockData(m_data + m_offset + m_blockSize, data);

	// Add size to block size
	m_blockSize += size;
}


}