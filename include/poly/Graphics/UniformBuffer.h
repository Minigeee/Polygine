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

class UniformBuffer
{
public:
	UniformBuffer();

	static Uint32 getUniformBlockAlignment();

	void bind(Uint32 index, Uint32 offset = 0xFFFFFFFFu, Uint32 size = 0);

	template <typename T>
	Uint32 pushData(const T& data);

	void setSize(Uint32 size);

	Uint32 getId() const;

	Uint32 getSize() const;

private:
	VertexBuffer m_uniformBuffer;
	Uint32 m_offset;
	Uint32 m_prevOffset;
	Uint32 m_prevSize;

	static Uint32 s_uniformBlockAlignment;
};


///////////////////////////////////////////////////////////
template <typename T, Uint32 N = 0>
struct UniformBufferType { };


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
	float m_padding[1];
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

}

#include <poly/Graphics/UniformBuffer.inl>

#endif