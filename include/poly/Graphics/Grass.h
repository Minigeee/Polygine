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

	void setGrassSpacing(float spacing);

	void setGrassWidth(float width);

	void setGrassHeight(float height);

	void setLodDistance(float dist, Uint32 lodLevel);

	void setColor(float r, float g, float b);

	void setColor(const Vector3f& color);

	void setColorMap(Texture* cmap);

	void setDensityMap(Texture* dmap);

	void setSizeMap(Texture* smap);

	float getGrassSpacing() const;

	float getGrassWidth() const;

	float getGrassHeight() const;

	float getLodDistance(Uint32 lodLevel) const;

	const Vector3f& getColor() const;

	Texture* getColorMap() const;

	Texture* getDensityMap() const;

	Texture* getSizeMap() const;

private:
	static Shader& getShader();

	static Shader s_shader;

private:
	Scene* m_scene;
	VertexArray m_vertexArray;
	VertexBuffer m_vertexBuffer;

	Terrain* m_terrain;
	Vector3f m_ambientColor;
	float m_grassWidth;
	float m_grassHeight;
	float m_grassSpacing;
	std::vector<float> m_lodDists;
	Vector3f m_color;
	Texture* m_colorMap;
	Texture* m_densityMap;
	Texture* m_sizeMap;
	bool m_hasDefaultColor;

	Clock m_clock;
};

}

#endif