#ifndef POLY_BILLBOARD_H
#define POLY_BILLBOARD_H

#include <poly/Graphics/Renderable.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

namespace poly
{

class Material;
class Texture;

class Billboard : public Renderable
{
public:
	Billboard();

	~Billboard();

	bool load(const std::string& fname);

	void setSize(const Vector2f& size);

	void setSize(float w, float h);

	void setOrigin(const Vector2f& origin);

	void setOrigin(float x, float y);

	Material* getMaterial() const;

	Texture* getTexture() const;

	const Vector2f& getSize() const;

	const Vector2f& getOrigin() const;

	static VertexArray& getVertexArray();

	static Shader& getDefaultShader();

private:
	void updateBoundingVolumes();

private:
	Material* m_material;
	Texture* m_texture;
	Vector2f m_size;
	Vector2f m_origin;

	static VertexArray s_vertexArray;
	static Shader s_shader;
};

}

#endif