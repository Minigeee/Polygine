#ifndef POLY_MODEL_H
#define POLY_MODEL_H

#include <poly/Core/DataTypes.h>

#include <poly/Math/Vector3.h>

#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

#include <string>
#include <vector>

namespace poly
{

struct Vertex
{
	Vertex();
	Vertex(const Vector3f& pos, const Vector3f& normal);
	Vertex(const Vector3f& pos, const Vector3f& normal, const Vector2f& texCoord);
	Vertex(const Vector3f& pos, const Vector3f& normal, const Colorf& color);
	Vertex(const Vector3f& pos, const Vector3f& normal, const Vector2f& texCoord, const Colorf& color);

	Vector3f m_position;
	Vector3f m_normal;
	Vector2f m_texCoord;
	Colorf m_color;
};


class Model
{
public:
	bool load(const std::string& fname);

	void create(const std::vector<Vertex>& vertices, BufferUsage usage = BufferUsage::Static);

	void setVertices(const std::vector<Vertex>& vertices);

	VertexArray& getVertexArray();

	const std::vector<Vertex>& getVertices() const;

private:
	VertexArray m_vertexArray;
	VertexBuffer m_vertexBuffer;
	std::vector<Vertex> m_vertices;
};

}

#endif