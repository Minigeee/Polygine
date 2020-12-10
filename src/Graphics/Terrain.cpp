#include <poly/Core/Profiler.h>

#include <poly/Engine/Scene.h>

#include <poly/Graphics/Components.h>
#include <poly/Graphics/Terrain.h>

#include <poly/Math/Transform.h>
#include <poly/Math/Vector2.h>
#include <poly/Math/Vector4.h>

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
struct TerrainVertex
{
	TerrainVertex() = default;
	TerrainVertex(const Vector2f& pos, const Vector2f& nearTexCoord, const Vector2f& farTexCoord) :
		m_position		(pos),
		m_nearTexCoord	(nearTexCoord),
		m_farTexCoord	(farTexCoord)
	{ }

	Vector2f m_position;
	Vector2f m_nearTexCoord;
	Vector2f m_farTexCoord;
};


///////////////////////////////////////////////////////////
void addTerrainTriangle(
	std::vector<priv::TerrainVertex>& vertices,
	const Vector2f& p1, const Vector2f& p2, const Vector2f& p3,
	const Vector2f& t1, const Vector2f& t2, const Vector2f& t3)
{
	Vector2f nearTexCoord = (p1 + p2 + p3) / 3.0f;
	Vector2f farTexCoord = (t1 + t2 + t3) / 3.0f;

	vertices.push_back(priv::TerrainVertex(p1, nearTexCoord, farTexCoord));
	vertices.push_back(priv::TerrainVertex(p2, nearTexCoord, farTexCoord));
	vertices.push_back(priv::TerrainVertex(p3, nearTexCoord, farTexCoord));
}


}


///////////////////////////////////////////////////////////
Shader Terrain::s_shader;


///////////////////////////////////////////////////////////
Shader& Terrain::getShader()
{
	if (!s_shader.getId())
	{
		s_shader.load("shaders/terrain.vert", Shader::Vertex);
		s_shader.load("shaders/terrain.frag", Shader::Fragment);
		s_shader.compile();
	}

	return s_shader;
}


///////////////////////////////////////////////////////////
Terrain::Terrain() :
	m_size					(0.0f),
	m_height				(0.0f),
	m_tileScale				(0.0f),
	m_lodScale				(0.0f),
	m_maxDist				(0.0f),
	m_normalMapData			(0),
	m_instanceBufferOffset	(0)
{

}


///////////////////////////////////////////////////////////
Terrain::~Terrain()
{
	if (m_normalMapData)
		free(m_normalMapData);
}


///////////////////////////////////////////////////////////
void Terrain::init(Scene* scene)
{
	m_scene = scene;
}


///////////////////////////////////////////////////////////
void Terrain::create(float size, float height, float tileScale, float lodScale, float maxDist)
{
	// Only create once
	if (m_instanceBuffer.getId()) return;

	m_size = size;
	m_height = height;
	m_tileScale = tileScale;
	m_lodScale = lodScale;
	m_maxDist = maxDist;

	std::vector<priv::TerrainVertex> vertices;

	Uint32 edgeTileOffset = 0;
	float tileSize = 2.0f;

	// Create vertices for tiles
	for (int i = 0; i < 2; ++i)
	{
		for (int row = -4; row < 4; row += 2)
		{
			for (int col = -4; col < 4; col += 2)
			{
				// Vertex positions
				float l = col * tileSize;
				float r = (col + 1) * tileSize;
				float t = row * tileSize;
				float b = (row + 1) * tileSize;
				Vector2f m = Vector2f(l, t) + 0.5f * tileSize;

				// Top left square
				priv::addTerrainTriangle(
					vertices,
					Vector2f(l, t), Vector2f(l, b), m,
					Vector2f(l, t), Vector2f(l, b + tileSize), Vector2f(r, b));

				priv::addTerrainTriangle(
					vertices,
					Vector2f(l, b), Vector2f(r, b), m,
					Vector2f(l, t), Vector2f(l, b + tileSize), Vector2f(r, b));

				priv::addTerrainTriangle(
					vertices,
					Vector2f(r, b), Vector2f(r, t), m,
					Vector2f(l, t), Vector2f(r + tileSize, t), Vector2f(r, b));

				priv::addTerrainTriangle(
					vertices,
					Vector2f(r, t), Vector2f(l, t), m,
					Vector2f(l, t), Vector2f(r + tileSize, t), Vector2f(r, b));

				// Top right square
				++col;
				l = col * tileSize;
				r = (col + 1) * tileSize;
				t = row * tileSize;
				b = (row + 1) * tileSize;
				m = Vector2f(l, t) + 0.5f * tileSize;

				priv::addTerrainTriangle(
					vertices,
					Vector2f(l, t), Vector2f(l, b), m,
					Vector2f(l - tileSize, t), Vector2f(r, t), Vector2f(l, b));

				priv::addTerrainTriangle(
					vertices,
					Vector2f(l, b), Vector2f(r, b), m,
					Vector2f(r, t), Vector2f(r, b + tileSize), Vector2f(l, b));

				priv::addTerrainTriangle(
					vertices,
					Vector2f(r, b), Vector2f(r, t), m,
					Vector2f(r, t), Vector2f(r, b + tileSize), Vector2f(l, b));

				priv::addTerrainTriangle(
					vertices,
					Vector2f(r, t), Vector2f(l, t), m,
					Vector2f(l - tileSize, t), Vector2f(r, t), Vector2f(l, b));

				// Bot left square
				--col, ++row;
				l = col * tileSize;
				r = (col + 1) * tileSize;
				t = row * tileSize;
				b = (row + 1) * tileSize;
				m = Vector2f(l, t) + 0.5f * tileSize;

				priv::addTerrainTriangle(
					vertices,
					Vector2f(l, t), Vector2f(l, b), m,
					Vector2f(l, t - tileSize), Vector2f(l, b), Vector2f(r, t));

				if (i == 1 && row == 3)
				{
					priv::addTerrainTriangle(
						vertices,
						Vector2f(l, b), Vector2f(l + 0.5f * tileSize, b), m,
						Vector2f(l, b), Vector2f(r + tileSize, b), Vector2f(r, t));

					priv::addTerrainTriangle(
						vertices,
						Vector2f(l + 0.5f * tileSize, b), Vector2f(r, b), m,
						Vector2f(l, b), Vector2f(r + tileSize, b), Vector2f(r, t));
				}
				else
				{
					priv::addTerrainTriangle(
						vertices,
						Vector2f(l, b), Vector2f(r, b), m,
						Vector2f(l, b), Vector2f(r + tileSize, b), Vector2f(r, t));
				}

				priv::addTerrainTriangle(
					vertices,
					Vector2f(r, b), Vector2f(r, t), m,
					Vector2f(l, b), Vector2f(r + tileSize, b), Vector2f(r, t));

				priv::addTerrainTriangle(
					vertices,
					Vector2f(r, t), Vector2f(l, t), m,
					Vector2f(l, t - tileSize), Vector2f(l, b), Vector2f(r, t));

				// Bot right square
				++col;
				l = col * tileSize;
				r = (col + 1) * tileSize;
				t = row * tileSize;
				b = (row + 1) * tileSize;
				m = Vector2f(l, t) + 0.5f * tileSize;

				priv::addTerrainTriangle(
					vertices,
					Vector2f(l, t), Vector2f(l, b), m,
					Vector2f(l, b - tileSize), Vector2f(r, b), Vector2f(l, t));

				if (i == 1 && row == 3)
				{
					priv::addTerrainTriangle(
						vertices,
						Vector2f(l, b), Vector2f(l + 0.5f * tileSize, b), m,
						Vector2f(l, b - tileSize), Vector2f(r, b), Vector2f(l, t));

					priv::addTerrainTriangle(
						vertices,
						Vector2f(l + 0.5f * tileSize, b), Vector2f(r, b), m,
						Vector2f(l, b - tileSize), Vector2f(r, b), Vector2f(l, t));
				}
				else
				{
					priv::addTerrainTriangle(
						vertices,
						Vector2f(l, b), Vector2f(r, b), m,
						Vector2f(l, b - tileSize), Vector2f(r, b), Vector2f(l, t));
				}

				priv::addTerrainTriangle(
					vertices,
					Vector2f(r, b), Vector2f(r, t), m,
					Vector2f(r, b), Vector2f(r, t - tileSize), Vector2f(l, t));

				priv::addTerrainTriangle(
					vertices,
					Vector2f(r, t), Vector2f(l, t), m,
					Vector2f(r, b), Vector2f(r, t - tileSize), Vector2f(l, t));

				// Reset row and column
				--row, --col;
			}
		}

		if (i == 0)
			edgeTileOffset = vertices.size();
	}

	// Upload to vertex buffer
	m_normalBuffer.create(&vertices[0], edgeTileOffset);
	m_edgeBuffer.create(&vertices[edgeTileOffset], vertices.size() - edgeTileOffset);

	// Add to vertex arrays
	m_normalTile.addBuffer(m_normalBuffer, 0, 2, sizeof(priv::TerrainVertex), 0 * sizeof(float));
	m_normalTile.addBuffer(m_normalBuffer, 1, 2, sizeof(priv::TerrainVertex), 2 * sizeof(float));
	m_normalTile.addBuffer(m_normalBuffer, 2, 2, sizeof(priv::TerrainVertex), 4 * sizeof(float));

	m_edgeTile.addBuffer(m_edgeBuffer, 0, 2, sizeof(priv::TerrainVertex), 0 * sizeof(float));
	m_edgeTile.addBuffer(m_edgeBuffer, 1, 2, sizeof(priv::TerrainVertex), 2 * sizeof(float));
	m_edgeTile.addBuffer(m_edgeBuffer, 2, 2, sizeof(priv::TerrainVertex), 4 * sizeof(float));

	// Create tile layout
	createTileLayout();

	// Create instance buffer
	m_instanceBuffer.create((Matrix4f*)0, 32768, BufferUsage::Dynamic);
}


///////////////////////////////////////////////////////////
void Terrain::createTileLayout()
{
	// Clear tile lists
	m_normalTiles.clear();
	m_edgeTiles.clear();
	m_lodDists.clear();

	// Set up lod rings
	int lodLevel = 0;
	Uint32 numTiles = 0;
	bool changedLodLevel = false;

	float tileSize = 16.0f * m_tileScale;
	Vector2f tl = Vector2f(-tileSize);

	float lodDists[] = { 20.0f, 100.0f, 200.0f };

	while (-tl.x < m_maxDist)
	{
		numTiles += 2;

		// Get top left
		Vector2f tr = Vector2f(tl.x + (numTiles - 1) * tileSize, tl.y);
		Vector2f bl = Vector2f(tl.x, tl.y + (numTiles - 1) * tileSize);

		// Horizontal ring row
		for (Uint32 c = 0; c < numTiles; ++c)
		{
			TerrainTile tile;
			tile.m_position = Vector2f(tl.x + ((float)c + 0.5f) * tileSize, tl.y + 0.5f * tileSize);
			tile.m_scale = powf(2.0f, (float)lodLevel) * m_tileScale;
			tile.m_lod = lodLevel;
			tile.m_boundingBox.m_min = Vector3f(tile.m_position.x - 0.5f * tileSize, 0.0f, tile.m_position.y - 0.5f * tileSize);
			tile.m_boundingBox.m_max = Vector3f(tile.m_position.x + 0.5f * tileSize, m_height, tile.m_position.y + 0.5f * tileSize);

			if (changedLodLevel && c != 0 && c != numTiles - 1)
			{
				tile.m_rotation = 0.0f;
				m_edgeTiles.push_back(tile);
			}
			else
			{
				tile.m_rotation = 0.0f;
				m_normalTiles.push_back(tile);
			}

			tile.m_position = Vector2f(tl.x + ((float)c + 0.5f) * tileSize, bl.y + 0.5f * tileSize);
			tile.m_scale = powf(2.0f, (float)lodLevel) * m_tileScale;
			tile.m_lod = lodLevel;
			tile.m_boundingBox.m_min = Vector3f(tile.m_position.x - 0.5f * tileSize, 0.0f, tile.m_position.y - 0.5f * tileSize);
			tile.m_boundingBox.m_max = Vector3f(tile.m_position.x + 0.5f * tileSize, m_height, tile.m_position.y + 0.5f * tileSize);

			if (changedLodLevel && c != 0 && c != numTiles - 1)
			{
				tile.m_rotation = 180.0f;
				m_edgeTiles.push_back(tile);
			}
			else
			{
				tile.m_rotation = 0.0f;
				m_normalTiles.push_back(tile);
			}
		}

		// Vertical ring row
		for (Uint32 r = 1; r < numTiles - 1; ++r)
		{
			TerrainTile tile;
			tile.m_position = Vector2f(tl.x + 0.5f * tileSize, tl.y + ((float)r + 0.5f) * tileSize);
			tile.m_scale = powf(2.0f, (float)lodLevel) * m_tileScale;
			tile.m_lod = lodLevel;
			tile.m_boundingBox.m_min = Vector3f(tile.m_position.x - 0.5f * tileSize, 0.0f, tile.m_position.y - 0.5f * tileSize);
			tile.m_boundingBox.m_max = Vector3f(tile.m_position.x + 0.5f * tileSize, m_height, tile.m_position.y + 0.5f * tileSize);

			if (changedLodLevel)
			{
				tile.m_rotation = 90.0f;
				m_edgeTiles.push_back(tile);
			}
			else
			{
				tile.m_rotation = 0.0f;
				m_normalTiles.push_back(tile);
			}

			tile.m_position = Vector2f(tr.x + 0.5f * tileSize, tl.y + ((float)r + 0.5f) * tileSize);
			tile.m_scale = powf(2.0f, (float)lodLevel) * m_tileScale;
			tile.m_lod = lodLevel;
			tile.m_boundingBox.m_min = Vector3f(tile.m_position.x - 0.5f * tileSize, 0.0f, tile.m_position.y - 0.5f * tileSize);
			tile.m_boundingBox.m_max = Vector3f(tile.m_position.x + 0.5f * tileSize, m_height, tile.m_position.y + 0.5f * tileSize);

			if (changedLodLevel)
			{
				tile.m_rotation = 270.0f;
				m_edgeTiles.push_back(tile);
			}
			else
			{
				tile.m_rotation = 0.0f;
				m_normalTiles.push_back(tile);
			}
		}

		if (changedLodLevel)
			changedLodLevel = false;

		// Update lod level
		if (lodLevel < 3 && -tl.x > lodDists[lodLevel] * m_lodScale + tileSize && numTiles % 4 == 0)
		{
			++lodLevel;
			tileSize *= 2.0f;
			numTiles /= 2;
			changedLodLevel = true;

			m_lodDists.push_back(-tl.x - tileSize * 0.25f);
		}

		tl -= Vector2f(tileSize);
	}

	m_lodDists.push_back(-tl.x);
}


///////////////////////////////////////////////////////////
void Terrain::render(Camera& camera)
{
	ASSERT(m_scene, "The terrain must be initialized before using, by calling the init() function");

	START_PROFILING_FUNC;

	std::vector<TerrainTile*> normalTiles, edgeTiles;

	// Cull and decide which tiles are in the frustum
	const Frustum& frustum = camera.getFrustum();

	// Snap terrain position
	float resolutionFactor = 16.0f * m_tileScale;
	Vector3f pos = camera.getPosition();
	pos.x = roundf(pos.x / resolutionFactor) * resolutionFactor;
	pos.z = roundf(pos.z / resolutionFactor) * resolutionFactor;
	pos.y = 0.0f;

	for (Uint32 i = 0; i < m_normalTiles.size(); ++i)
	{
		BoundingBox bbox = m_normalTiles[i].m_boundingBox;
		bbox.m_min += pos;
		bbox.m_max += pos;

		if (frustum.contains(bbox))
			normalTiles.push_back(&m_normalTiles[i]);
	}

	for (Uint32 i = 0; i < m_edgeTiles.size(); ++i)
	{
		BoundingBox bbox = m_edgeTiles[i].m_boundingBox;
		bbox.m_min += pos;
		bbox.m_max += pos;

		if (frustum.contains(bbox))
			edgeTiles.push_back(&m_edgeTiles[i]);
	}

	if (normalTiles.size() + edgeTiles.size() == 0) return;


	// Stream instance data
	Uint32 size = (normalTiles.size() + edgeTiles.size()) * sizeof(InstanceData);
	int flags = (int)MapBufferFlags::Write | (int)MapBufferFlags::Unsynchronized;

	// Choose different flags based on how much space is left
	if (m_instanceBufferOffset + size > m_instanceBuffer.getSize())
	{
		flags |= (int)MapBufferFlags::InvalidateBuffer;
		m_instanceBufferOffset = 0;
	}

	// Map the buffer
	InstanceData* buffer = (InstanceData*)m_instanceBuffer.map(m_instanceBufferOffset, size, flags);

	for (Uint32 i = 0; i < normalTiles.size(); ++i)
	{
		TerrainTile* tile = normalTiles[i];
		Vector3f tilePos = Vector3f(tile->m_position.x, 0.0f, tile->m_position.y) + pos;

		InstanceData& data = buffer[i];
		data.m_lodDist = m_lodDists[tile->m_lod];
		data.m_transform = toTransformMatrix(tilePos, Vector3f(0.0f, tile->m_rotation, 0.0f), Vector3f(tile->m_scale));
	}

	for (Uint32 i = 0; i < edgeTiles.size(); ++i)
	{
		TerrainTile* tile = edgeTiles[i];
		Vector3f tilePos = Vector3f(tile->m_position.x, 0.0f, tile->m_position.y) + pos;

		InstanceData& data = buffer[normalTiles.size() + i];
		data.m_lodDist = m_lodDists[tile->m_lod];
		data.m_transform = toTransformMatrix(tilePos, Vector3f(0.0f, tile->m_rotation, 0.0f), Vector3f(tile->m_scale));
	}

	// After pushing all instance data, unmap the buffer
	m_instanceBuffer.unmap();


	// Bind shader and set uniforms
	Shader& shader = getShader();

	shader.bind();
	shader.setUniform("u_projView", camera.getProjMatrix() * camera.getViewMatrix());
	shader.setUniform("u_cameraPos", camera.getPosition());

	// Apply directional lights
	int i = 0;
	m_scene->system<DirLightComponent>(
		[&](const Entity::Id id, DirLightComponent& light)
		{
			std::string prefix = "u_dirLights[" + std::to_string(i++) + "].";

			shader.setUniform(prefix + "diffuse", light.m_diffuse);
			shader.setUniform(prefix + "specular", light.m_specular);
			shader.setUniform(prefix + "direction", normalize(light.m_direction));
		}
	);
	shader.setUniform("u_numDirLights", i);

	// Terrain maps
	shader.setUniform("u_heightMap", 0);
	shader.setUniform("u_normalMap", 1);
	shader.setUniform("u_colorMap", 2);
	m_heightMap.bind(0);
	m_normalMap.bind(1);
	m_colorMap.bind(2);

	// Terrain parameters
	shader.setUniform("u_size", m_size);
	shader.setUniform("u_height", m_height);
	shader.setUniform("u_tileScale", m_tileScale);
	shader.setUniform("u_blendLodDist", m_lodDists[m_lodDists.size() - 2]);

	// Attach instance buffer and render
	m_normalTile.bind();
	m_normalTile.addBuffer(m_instanceBuffer, 3, 1, sizeof(InstanceData), m_instanceBufferOffset, 1);
	m_normalTile.addBuffer(m_instanceBuffer, 4, 4, sizeof(InstanceData), m_instanceBufferOffset + 4 + 0 * sizeof(Vector4f), 1);
	m_normalTile.addBuffer(m_instanceBuffer, 5, 4, sizeof(InstanceData), m_instanceBufferOffset + 4 + 1 * sizeof(Vector4f), 1);
	m_normalTile.addBuffer(m_instanceBuffer, 6, 4, sizeof(InstanceData), m_instanceBufferOffset + 4 + 2 * sizeof(Vector4f), 1);
	m_normalTile.addBuffer(m_instanceBuffer, 7, 4, sizeof(InstanceData), m_instanceBufferOffset + 4 + 3 * sizeof(Vector4f), 1);
	m_normalTile.draw(normalTiles.size());

	Uint32 offset = m_instanceBufferOffset + normalTiles.size() * sizeof(InstanceData);
	m_edgeTile.bind();
	m_edgeTile.addBuffer(m_instanceBuffer, 3, 1, sizeof(InstanceData), offset, 1);
	m_edgeTile.addBuffer(m_instanceBuffer, 4, 4, sizeof(InstanceData), offset + 4 + 0 * sizeof(Vector4f), 1);
	m_edgeTile.addBuffer(m_instanceBuffer, 5, 4, sizeof(InstanceData), offset + 4 + 1 * sizeof(Vector4f), 1);
	m_edgeTile.addBuffer(m_instanceBuffer, 6, 4, sizeof(InstanceData), offset + 4 + 2 * sizeof(Vector4f), 1);
	m_edgeTile.addBuffer(m_instanceBuffer, 7, 4, sizeof(InstanceData), offset + 4 + 3 * sizeof(Vector4f), 1);
	m_edgeTile.draw(edgeTiles.size());

	// Update buffer offset
	m_instanceBufferOffset += size;
}


///////////////////////////////////////////////////////////
void Terrain::setSize(float size)
{
	// Calculate change in scale
	Vector3f scale(size / m_size);
	scale.y = 1.0f;

	// Update size
	m_size = size;

	// Update normal map
	updateNormalMap(scale);
}


///////////////////////////////////////////////////////////
void Terrain::setHeight(float height)
{
	// Calculate change in scale
	Vector3f scale(1.0f);
	scale.y = height / m_height;

	// Update size
	m_height = height;

	// Update normal map
	updateNormalMap(scale);
}


///////////////////////////////////////////////////////////
void Terrain::setTileScale(float scale)
{
	m_tileScale = scale;

	// Update tile layout
	createTileLayout();
}


///////////////////////////////////////////////////////////
void Terrain::setLodScale(float scale)
{
	m_lodScale = scale;

	// Update tile layout
	createTileLayout();
}


///////////////////////////////////////////////////////////
void Terrain::setMaxDist(float dist)
{
	m_maxDist = dist;

	// Update tile layout
	createTileLayout();
}


///////////////////////////////////////////////////////////
void Terrain::setHeightMap(const Image& map)
{
	// Upload data to texture
	m_heightMap.create(map);

	// Iterate through data and generate normals
	Vector2u size = Vector2u(map.getWidth(), map.getHeight());
	if (!m_normalMapData)
	m_normalMapData = (Vector3f*)malloc(size.x * size.y * sizeof(Vector3f));

	Vector2f sizeFactor = m_size / Vector2f(size);

	for (Uint32 r = 0, i = 0; r < size.y; ++r)
	{
		for (Uint32 c = 0; c < size.x; ++c, ++i)
		{
			// Calculate normal
			float h01 = *(float*)map.getPixel(r, c - (c == 0 ? 0 : 1)) * m_height;
			float h21 = *(float*)map.getPixel(r, c + (c == size.x - 1 ? 0 : 1)) * m_height;
			float h10 = *(float*)map.getPixel(r + (r == size.y - 1 ? 0 : 1), c) * m_height;
			float h12 = *(float*)map.getPixel(r - (r == 0 ? 0 : 1), c) * m_height;

			Vector3f v1(sizeFactor.x, h21 - h01, 0.0f);
			Vector3f v2(0.0f, h12 - h10, -sizeFactor.y);
			Vector3f normal = normalize(cross(v1, v2));

			m_normalMapData[i] = normal;
		}
	}

	// Upload normal data
	m_normalMap.create(m_normalMapData, PixelFormat::Rgb, size.x, size.y, 0, GLType::Float);
}


///////////////////////////////////////////////////////////
void Terrain::updateNormalMap(const Vector3f& scale)
{
	if (!m_normalMap.getId()) return;

	// Get normal transform matrix
	Matrix3f transform = Matrix3f(transpose(inverse(toTransformMatrix(Vector3f(0.0f), Vector3f(0.0f), scale))));

	// Multiply every normal by transform
	Uint32 size = m_normalMap.getWidth() * m_normalMap.getHeight();
	for (Uint32 i = 0; i < size; ++i)
		m_normalMapData[i] = transform * m_normalMapData[i];

	// Update texture data
	m_normalMap.update(m_normalMapData);
}


///////////////////////////////////////////////////////////
void Terrain::setColorMap(const Image& map)
{
	// Upload data to texture
	m_colorMap.create(map);
}


///////////////////////////////////////////////////////////
float Terrain::getSize() const
{
	return m_size;
}


///////////////////////////////////////////////////////////
float Terrain::getHeight() const
{
	return m_height;
}


///////////////////////////////////////////////////////////
float Terrain::getTileScale() const
{
	return m_tileScale;
}


///////////////////////////////////////////////////////////
float Terrain::getLodScale() const
{
	return m_lodScale;
}


///////////////////////////////////////////////////////////
float Terrain::getMaxDist() const
{
	return m_maxDist;
}


///////////////////////////////////////////////////////////
const Texture& Terrain::getHeightMap() const
{
	return m_heightMap;
}


///////////////////////////////////////////////////////////
const Texture& Terrain::getColorMap() const
{
	return m_colorMap;
}


///////////////////////////////////////////////////////////
const Texture& Terrain::getNormalMap() const
{
	return m_normalMap;
}


}