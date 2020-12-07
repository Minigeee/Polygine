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

	~Terrain();

	void init(Scene* scene) override;

	void create(float size, float height, float tileScale = 1.0f, float lodScale = 1.0f, float maxDist = 800.0f);

	void render(Camera& camera) override;

	void setSize(float size);

	void setHeight(float height);

	void setTileScale(float scale);

	void setLodScale(float scale);

	void setMaxDist(float maxDist);

	void setHeightMap(const Image& map);

	void setColorMap(const Image& map);

	float getSize() const;

	float getHeight() const;

	float getTileScale() const;

	float getLodScale() const;

	float getMaxDist() const;

	const Texture& getHeightMap() const;

	const Texture& getColorMap() const;

	const Texture& getNormalMap() const;

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

	struct InstanceData
	{
		float m_lodDist;
		Matrix4f m_transform;
	};

	///////////////////////////////////////////////////////////
	/// \brief Create ring layout of terrain, depends on tile scale, lod scale, and max distance
	///
	///////////////////////////////////////////////////////////
	void createTileLayout();

	///////////////////////////////////////////////////////////
	/// \brief Update normal map, depends on height and size
	///
	///////////////////////////////////////////////////////////
	void updateNormalMap(const Vector3f& scale);

private:
	Scene* m_scene;
	float m_size;
	float m_height;
	float m_tileScale;
	float m_lodScale;
	float m_maxDist;

	Texture m_heightMap;
	Texture m_normalMap;
	Texture m_colorMap;
	Vector3f* m_normalMapData;

	VertexArray m_normalTile;
	VertexArray m_edgeTile;
	VertexBuffer m_normalBuffer;
	VertexBuffer m_edgeBuffer;
	VertexBuffer m_instanceBuffer;
	Uint32 m_instanceBufferOffset;

	std::vector<TerrainTile> m_normalTiles;
	std::vector<TerrainTile> m_edgeTiles;
	std::vector<float> m_lodDists;

	static Shader s_shader;
};

}

#endif