#ifndef POLY_VERTEX_ARRAY_H
#define POLY_VERTEX_ARRAY_H

#include <poly/Core/DataTypes.h>

#include <poly/Graphics/VertexBuffer.h>

namespace poly
{

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

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

#ifndef DOXYGEN_SKIP
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;
	VertexArray(VertexArray&&) = default;
	VertexArray& operator=(VertexArray&&) = default;
#endif

	void bind();

	void addBuffer(VertexBuffer& buffer, Uint32 index, Uint32 size, Uint32 stride = 0, Uint32 offset = 0, Uint32 divisor = 0);

	void draw(Uint32 instances = 1, Uint32 offset = 0);

	void setDrawMode(DrawMode mode);

	Uint32 getId() const;

	Uint32 getNumVertices() const;

	DrawMode getDrawMode() const;

	bool hasElementBuffer() const;

private:
	Uint32 m_id;
	Uint32 m_numVertices;
	DrawMode m_drawMode;
	bool m_hasElementBuffer;

	static Uint32 currentBound;
};

}

#endif