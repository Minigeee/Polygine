#ifndef POLY_VERTEX_BUFFER_H
#define POLY_VERTEX_BUFFER_H

#include <poly/Core/DataTypes.h>

#include <poly/Graphics/GLType.h>

#include <vector>

namespace poly
{

enum class BufferTarget
{
	Array = 0,
	Element,
	TransformFeedback,
	Uniform
};

enum class BufferUsage
{
	Static	= 0x88E4,
	Dynamic = 0x88E8,
	Stream	= 0x88E0
};

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	VertexBuffer(const VertexBuffer& other);

	VertexBuffer& operator=(const VertexBuffer& other);

	void bind(BufferTarget target = BufferTarget::Array);

	void bind(BufferTarget target, Uint32 index);

	template <typename T>
	void create(const std::vector<T>& data, BufferUsage usage = BufferUsage::Static, GLType dtype = GLType::Unknown);

	template <typename T>
	void create(const T* data, Uint32 num, BufferUsage usage = BufferUsage::Static, GLType dtype = GLType::Unknown);

	template <typename T>
	void update(const std::vector<T>& data, Uint32 offset = 0);

	template <typename T>
	void update(const T* data, Uint32 num, Uint32 offset = 0);

	void update(const VertexBuffer& buffer, Uint32 offset = 0);

	Uint32 getId() const;

	Uint32 getSize() const;

	BufferUsage getUsage() const;

	BufferTarget getTarget() const;

	GLType getDataType() const;

private:
	template <typename T>
	void setDataType();

	template <typename T>
	bool checkDataType() const;

	void bufferData(const void* data, Uint32 size, BufferUsage usage);

	void bufferSubData(const void* data, Uint32 size, Uint32 offset);

	Uint32 m_id;
	Uint32 m_size;
	BufferUsage m_usage;
	BufferTarget m_target;
	GLType m_type;

	static Uint32 currentBound[4];
};

}

#include <poly/Graphics/VertexBuffer.inl>

#endif