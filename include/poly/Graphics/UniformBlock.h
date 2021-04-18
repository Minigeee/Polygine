#ifndef POLY_UNIFORM_BLOCK_H
#define POLY_UNIFORM_BLOCK_H

#include <poly/Graphics/VertexBuffer.h>

namespace poly
{

class UniformBlock
{
public:
	UniformBlock();

	~UniformBlock();

	void bind(Uint32 index);

	template <typename T>
	void addData(const T& data, Uint32 padding = 0);

	void update();

	void setBufferSize(Uint32 size);

	Uint32 getId() const;

	Uint32 getBufferSize() const;

	Uint32 getBlockSize() const;

private:
	VertexBuffer m_vertexBuffer;

	Uint8* m_data;
	Uint32 m_offset;
	Uint32 m_prevOffset;
	Uint32 m_blockSize;
	bool m_bufferReset;
};

}

#include <poly/Graphics/UniformBlock.inl>

#endif