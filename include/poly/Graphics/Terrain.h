#ifndef POLY_TERRAIN_H
#define POLY_TERRAIN_H

#include <poly/Graphics/Shader.h>
#include <poly/Graphics/RenderSystem.h>
#include <poly/Graphics/Texture.h>
#include <poly/Graphics/VertexArray.h>
#include <poly/Graphics/VertexBuffer.h>

namespace poly
{

class Terrain : public RenderSystem
{
public:
	Terrain();

	void init(Scene* scene) override;

	void create(float size, float height, float resolution = 1.0f, float lodScale = 1.0f, float maxDist = 800.0f);

	void render(Camera& camera) override;

	void setSize(float size);

	void setHeight(float height);

	void setResolution(float resolution);

	void setLodScale(float scale);

	void setMaxDist(float maxDist);

	void setHeightMap(Texture* map);

	void setColorMap(Texture* map);

private:
	static Shader& getShader();

	struct TerrainTile
	{
		BoundingBox m_boundingBox;
		Vector2f m_position;
		float m_rotation;
		float m_scale;
		Uint32 m_lod;
	};

private:
	Scene* m_scene;
	float m_size;
	float m_height;
	float m_resolution;
	float m_lodScale;
	float m_maxDist;

	Texture* m_heightMap;
	Texture m_normalMap;
	Texture* m_colorMap;

	VertexArray m_normalTile;
	VertexArray m_edgeTile;
	VertexBuffer m_normalBuffer;
	VertexBuffer m_edgeBuffer;
	VertexBuffer m_instanceBuffer;
	Uint32 m_instanceBufferOffset;

	std::vector<TerrainTile> m_normalTiles;
	std::vector<TerrainTile> m_edgeTiles;

	static Shader s_shader;
};

}

#endif