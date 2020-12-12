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

	void setZenithColor(const Vector3f& color);

	void setHorizonColor(const Vector3f& color);

	void setGroundColor(const Vector3f& color);

	void setBloomColor(const Vector3f& color);

	void setBloomSize(float size);

	void setLightStrength(float strength);

	void setTopRadius(float radius);

	void setBotRadius(float radius);

	void setAltitude(float alt);

	const Vector3f& getZenithColor() const;

	const Vector3f& getHorizonColor() const;

	const Vector3f& getGroundColor() const;

	const Vector3f& getBloomColor() const;

	float getBloomSize() const;

	float getLightStrength() const;

	float getTopRadius() const;

	float getBotRadius() const;

	float getAltitude() const;

private:
	static Shader s_shader;

	Shader& getShader();

private:
	Scene* m_scene;

	Vector3f m_zenithColor;
	Vector3f m_horizonColor;
	Vector3f m_groundColor;
	Vector3f m_bloomColor;
	float m_bloomSize;
	float m_lightStrength;

	float m_topRadius;
	float m_botRadius;
	float m_altitude;
};

}

#endif