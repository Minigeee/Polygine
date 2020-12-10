#ifndef POLY_SKYBOX_H
#define POLY_SKYBOX_H

#include <poly/Graphics/RenderSystem.h>
#include <poly/Graphics/Shader.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

#include <poly/Math/Vector3.h>

namespace poly
{

class ProceduralSkybox;

class Skybox
{
	friend ProceduralSkybox;

private:
	static VertexArray s_vertexArray;
	static VertexBuffer s_vertexBuffer;

	static VertexArray& getVertexArray();
};


class ProceduralSkybox : public RenderSystem
{
public:
	ProceduralSkybox();

	void init(Scene* scene) override;

	void render(Camera& camera) override;

private:
	static Shader s_shader;

	Shader& getShader();

private:
	Scene* m_scene;

	Vector3f m_zenithColor;
	Vector3f m_horizonColor;
	Vector3f m_groundColor;
	Vector3f m_bloomColor;
	float m_bloomStrength;
	float m_lightStrength;

	float m_topRadius;
	float m_botRadius;
	float m_altitude;
};

}

#endif