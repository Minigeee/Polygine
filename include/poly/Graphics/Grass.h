#ifndef POLY_GRASS_H
#define POLY_GRASS_H

#include <poly/Core/Clock.h>

#include <poly/Graphics/RenderSystem.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/Terrain.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

namespace poly
{

class Grass : public RenderSystem
{
public:
	Grass();

	void init(Scene* scene) override;

	void render(Camera& camera) override;

	void setTerrain(Terrain* terrain);

	void setAmbientColor(const Vector3f& color);

private:
	static Shader& getShader();

	static Shader s_shader;

private:
	Scene* m_scene;
	VertexArray m_vertexArray;
	VertexBuffer m_vertexBuffer;

	Terrain* m_terrain;
	Vector3f m_ambientColor;

	Clock m_clock;
};

}

#endif