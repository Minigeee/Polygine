#include <poly/Core/Allocate.h>
#include <poly/Core/ObjectPool.h>
#include <poly/Core/Profiler.h>

#include <poly/Engine/Scene.h>

#include <poly/Graphics/Components.h>
#include <poly/Graphics/GLCheck.h>
#include <poly/Graphics/Lighting.h>
#include <poly/Graphics/Shadows.h>
#include <poly/Graphics/Terrain.h>

#include <poly/Graphics/Shaders/terrain.vert.h>
#include <poly/Graphics/Shaders/terrain.frag.h>

#include <poly/Math/Functions.h>
#include <poly/Math/Transform.h>
#include <poly/Math/Vector2.h>
#include <poly/Math/Vector4.h>

#include <chrono>


#define TEXTURE_CAST(x) reinterpret_cast<Texture*>(x)
#define TMAP_CAST(x) reinterpret_cast<TerrainMap*>(x)


namespace poly
{


///////////////////////////////////////////////////////////
Vector2u estimateCacheSize(Uint32 numLevels)
{
	// Get the number of required tiles in the very worst case (this will be the size of cache)
	Uint32 numTiles = 1;
	numTiles += numLevels >= 1 ? 4 : 0;
	numTiles += numLevels >= 2 ? 9 * (numLevels - 2) : 0;

	// Get smallest rectangle that can fit the required number of tiles
	while (true)
	{
		// Keep track of biggest factor (to keep texture as square as possible)
		Uint32 biggestFactor = 1;

		// Get stopping point
		Uint32 stop = (Uint32)sqrtf((float)numTiles) + 1;

		for (Uint32 i = 2; i < stop; ++i)
		{
			if (numTiles % i == 0)
				biggestFactor = i;
		}

		// If biggest factor is not one, found a rectangle
		if (biggestFactor != 1)
			return Vector2u(numTiles / biggestFactor, biggestFactor);

		// Otherwise, increment num tiles by 1
		++numTiles;
	}

	//Get rid of warning
	return Vector2u();
}


///////////////////////////////////////////////////////////
TerrainMap::TerrainMap() :
	m_size			(0),
	m_tileSize		(0),
	m_format		(PixelFormat::Rgb),
	m_dataType		(GLType::Uint8)
{

}


///////////////////////////////////////////////////////////
void TerrainMap::create(Uint32 size, Uint32 tileSize, PixelFormat fmt, GLType dtype)
{
#ifndef NDEBUG
	{
		Uint32 factor = size / tileSize;
		ASSERT(size % tileSize == 0 && (factor & (factor - 1)) == 0, "Terrain map tile size must be a factor of map size and their quotient must be a power of 2");
	}
#endif

	m_size = size;
	m_tileSize = tileSize;
	m_format = fmt;
	m_dataType = dtype;
}


///////////////////////////////////////////////////////////
Texture& TerrainMap::getTexture()
{
	return m_texture;
}


///////////////////////////////////////////////////////////
Uint32 TerrainMap::getSize() const
{
	return m_size;
}


///////////////////////////////////////////////////////////
Uint32 TerrainMap::getTileSize() const
{
	return m_tileSize;
}


///////////////////////////////////////////////////////////
PixelFormat TerrainMap::getFormat() const
{
	return m_format;
}


///////////////////////////////////////////////////////////
GLType TerrainMap::getDataType() const
{
	return m_dataType;
}


///////////////////////////////////////////////////////////
void TerrainMap::onRequestTile(const std::function<bool(const Vector2i&, Uint32, Image&)>& func)
{
	m_loadFunc = func;
}


///////////////////////////////////////////////////////////
Image* TerrainMap::load(const Vector2i& pos, Uint32 level)
{
	// Set load image output
	Image* output = Pool<Image>::alloc();

	// Load image
	if (!m_loadFunc(pos, level, *output))
	{
		// Free image
		Pool<Image>::free(output);
		return NULL;
	}

	// Return image
	return output;
}


///////////////////////////////////////////////////////////
void TerrainMap::setCacheSize(Uint32 w, Uint32 h)
{
	// Create a new empty texture
	m_texture.create(NULL, m_format, w * m_tileSize, h * m_tileSize, 0, m_dataType);
}


///////////////////////////////////////////////////////////
Shader Terrain::s_shader;

///////////////////////////////////////////////////////////
Shader Terrain::s_tmapShader;


///////////////////////////////////////////////////////////
Shader& Terrain::getDefaultShader()
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
Shader& Terrain::getTerrainMapShader()
{
	if (!s_tmapShader.getId())
	{
		s_tmapShader.load("shaders/terrain_tmap.vert", Shader::Vertex);
		s_tmapShader.load("shaders/terrain_tmap.frag", Shader::Fragment);
		s_tmapShader.compile();
	}

	return s_tmapShader;
}


///////////////////////////////////////////////////////////
Terrain::Terrain() :
	m_shader				(0),
	m_size					(0.0f),
	m_maxHeight				(0.0f),
	m_viewpoint				(0.0f),
	m_heightMap				(0),
	m_normalMap				(0),
	m_tileSize				(0),
	m_usesTerrainMaps		(false),
	m_indexMapChanged		(false),
	m_highestMapLevel		(0),
	m_instanceDataOffset	(0),
	m_useCustomViewpoint	(false),
	m_baseScale				(1.0f)
{

}


///////////////////////////////////////////////////////////
Terrain::~Terrain()
{
	if (m_heightMap)
	{
		if (m_useCustomViewpoint)
			Pool<Texture>::free(TEXTURE_CAST(m_heightMap));
		else
			Pool<TerrainMap>::free(TMAP_CAST(m_heightMap));
	}

	if (m_normalMap)
	{
		if (m_useCustomViewpoint)
			Pool<Texture>::free(TEXTURE_CAST(m_normalMap));
		else
			Pool<TerrainMap>::free(TMAP_CAST(m_normalMap));
	}

	m_heightMap = 0;
	m_normalMap = 0;
}


///////////////////////////////////////////////////////////
void Terrain::init(Scene* scene)
{
	m_scene = scene;
}


///////////////////////////////////////////////////////////
void Terrain::create(float size, float maxHeight, float maxNodeSize, float viewDist)
{
	m_size = size;
	m_maxHeight = maxHeight;

	// Create buffer
	m_instanceBuffer.create((Vector4f*)NULL, 4 * 1024, BufferUsage::Stream);
	m_instanceDataOffset = 0;

	// Calculate lod related variables
	Uint32 lodLevel = 0;
	float currSize = size;
	while (currSize > 0.5f * maxNodeSize)
	{
		// Calculate number of nodes
		Uint32 numNodes = 1 << (lodLevel++);
		numNodes *= numNodes;

		// Add lod level
		m_lodLevels.push_back(LodLevel());
		m_lodLevels.back().m_heightBounds.resize(numNodes, Vector2<Uint16>(0, (Uint16)(m_maxHeight * 65535.0f)));

		// Update size
		currSize *= 0.5f;
	}

	// Calculate lod distances
	float distUnit = 2.0f * currSize;
	float prevDist = distUnit;

	for (int i = m_lodLevels.size() - 1; i >= 0; --i)
	{
		m_lodLevels[i].m_range = prevDist;
		prevDist += powf(2.0f, (float)(m_lodLevels.size() - i)) * distUnit;
	}

	// Create mesh
	constexpr Uint32 MESH_SIZE = 16;
	m_baseScale = 1.0f * currSize / (float)MESH_SIZE;
	std::vector<Vector2f> vertices((MESH_SIZE + 1) * (MESH_SIZE + 1));
	std::vector<Uint32> indices(6 * MESH_SIZE * MESH_SIZE);

	// Vertices
	for (Uint32 r = 0, i = 0; r < MESH_SIZE + 1; ++r)
	{
		for (Uint32 c = 0; c < MESH_SIZE + 1; ++c, ++i)
			vertices[i] = Vector2f(c, r) - (float)(MESH_SIZE / 2);
	}

	// Indices
	for (Uint32 r = 0, i = 0; r < MESH_SIZE; ++r)
	{
		for (Uint32 c = 0; c < MESH_SIZE; ++c, i += 6)
		{
			indices[i + 0] = (r + 0) * (MESH_SIZE + 1) + (c + 0);
			indices[i + 1] = (r + 1) * (MESH_SIZE + 1) + (c + 0);
			indices[i + 2] = (r + 0) * (MESH_SIZE + 1) + (c + 1);
			indices[i + 3] = (r + 1) * (MESH_SIZE + 1) + (c + 0);
			indices[i + 4] = (r + 1) * (MESH_SIZE + 1) + (c + 1);
			indices[i + 5] = (r + 0) * (MESH_SIZE + 1) + (c + 1);
		}
	}

	// Vertex buffers
	m_vertexBuffer.create(vertices);
	m_indexBuffer.create(indices);

	// Vertex array
	m_vertexArray.addBuffer(m_vertexBuffer, 0, 2);
	m_vertexArray.setElementBuffer(m_indexBuffer);
}


///////////////////////////////////////////////////////////
void Terrain::render(Camera& camera, RenderPass pass, const RenderSettings& settings)
{
	// The terrain should be rendered regardless of render pass

	ASSERT(m_scene, "The terrain must be initialized before using, by calling the init() function");

	// Load shader if none given yet
	if (!m_shader)
	{
		m_shader = m_usesTerrainMaps ? &getTerrainMapShader() : &getDefaultShader();

		// Set lod ranges a single time
		m_shader->bind();
		for (Uint32 i = 0; i < m_lodLevels.size(); ++i)
			m_shader->setUniform("u_lodRanges[" + std::to_string(i) + ']', m_lodLevels[i].m_range);
		m_shader->setUniform("u_lodRanges[" + std::to_string(m_lodLevels.size()) + ']', 0.0f);
	}

	START_PROFILING_FUNC;


	// Only update viewpoint for default render passes
	if (pass == RenderPass::Default && !m_useCustomViewpoint)
	{
		const Vector3f& cameraPos = camera.getPosition();
		m_viewpointChanged = m_viewpoint != cameraPos;
		m_viewpoint = cameraPos;
	}

	// Load terrain maps
	if (m_usesTerrainMaps)
	{
		if (m_viewpointChanged)
		{
			loadMapTiles(Vector2u(0), 0);
			m_viewpointChanged = false;
		}

		// Check if any load threads are complete
		if (m_loadThreads.size())
		{
			// Iterate backwards
			for (int i = m_loadThreads.size() - 1; i >= 0; --i)
			{
				AsyncTileObj& asyncObj = m_loadThreads[i];

				// Check if thread is complete
				if (asyncObj.m_future.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready)
					continue;

				// Thread is complete, get image data
				Image* img = asyncObj.m_future.get();

				// Push to testure
				asyncObj.m_cacheTexture->update(img->getData(), (Vector2u)asyncObj.m_cachePos * m_tileSize, Vector2u(m_tileSize));

				// Update mapping
				m_loadedMapTiles[asyncObj.m_nodeData] = asyncObj.m_cachePos;

				// Get bounds of tile in the index map
				Uint32 pixelsPerTile = 1 << (m_highestMapLevel - (Uint32)asyncObj.m_nodeData.z);
				Uint32 sr = asyncObj.m_nodeData.x * pixelsPerTile;
				Uint32 sc = asyncObj.m_nodeData.y * pixelsPerTile;
				Uint32 fr = (asyncObj.m_nodeData.x + 1) * pixelsPerTile;
				Uint32 fc = (asyncObj.m_nodeData.y + 1) * pixelsPerTile;

				// Update redirect location
				for (Uint32 r = sr; r < fr; ++r)
				{
					for (Uint32 c = sc; c < fc; ++c)
						m_indexMapImg[r][c] = asyncObj.m_cachePos;
				}

				// Mark index map as changed
				m_indexMapChanged = true;

				// Check if this is a height tile
				if (asyncObj.m_cacheTexture == &TMAP_CAST(m_heightMap)->m_texture)
				{
					// Don't free image yet, use it to create normal tile
					Uint32 numNodesPerEdge = 1 << asyncObj.m_nodeData.z;
					float nodeSize = m_size / (float)numNodesPerEdge;

					asyncObj.m_future = std::async(std::launch::async, &Terrain::createNormalTile, this, img, nodeSize);
					asyncObj.m_cacheTexture = &TMAP_CAST(m_normalMap)->m_texture;
					m_loadThreads.push_back(std::move(asyncObj));
				}
				else
				{
					// This is another tile, free image
					Pool<Image>::free(img);

					// Remove async object from list
					if (i != m_loadThreads.size())
						m_loadThreads[i] = std::move(m_loadThreads.back());
					m_loadThreads.pop_back();
				}
			}
		}

		// Update index map
		if (m_indexMapChanged)
		{
			// Update texture
			m_indexMap.update(m_indexMapImg.getData());

			m_indexMapChanged = false;
		}
	}

	// Get lod nodes
	std::vector<Vector4f> renderList;
	addLodNodes(Vector2u(0), 0, camera.getFrustum(), renderList);

	// Quit if no nodes are being rendered
	if (!renderList.size()) return;


	// Stream to instance buffer
	Uint32 size = renderList.size() * sizeof(Vector4f);
	MapBufferFlags flags = MapBufferFlags::Write | MapBufferFlags::Unsynchronized;

	// Choose different flags based on how much space is left
	if (m_instanceDataOffset + size > m_instanceBuffer.getSize())
	{
		flags |= MapBufferFlags::InvalidateBuffer;
		m_instanceDataOffset = 0;
	}

	// Map the buffer
	Vector4f* buffer = (Vector4f*)m_instanceBuffer.map(m_instanceDataOffset, size, flags);

	for (Uint32 i = 0; i < renderList.size(); ++i)
		buffer[i] = renderList[i];

	// After pushing all instance data, unmap the buffer
	m_instanceBuffer.unmap();

	// Update offset
	Uint32 offset = m_instanceDataOffset;
	m_instanceDataOffset += renderList.size() * sizeof(Vector4f);


	m_shader->bind();

	// Terrain
	m_shader->setUniform("u_size", m_size);
	m_shader->setUniform("u_maxHeight", m_maxHeight);

	// Textures
	Texture& hmap = m_usesTerrainMaps ? TMAP_CAST(m_heightMap)->m_texture : *TEXTURE_CAST(m_heightMap);
	Texture& nmap = m_usesTerrainMaps ? TMAP_CAST(m_normalMap)->m_texture : *TEXTURE_CAST(m_normalMap);
	m_shader->setUniform("u_heightMap", hmap);
	m_shader->setUniform("u_normalMap", nmap);
	if (m_usesTerrainMaps)
		m_shader->setUniform("u_indexMap", m_indexMap);

	// Camera
	camera.apply(m_shader);

	// Enable depth testing
	glCheck(glEnable(GL_DEPTH_TEST));

	// Single side render
	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glCullFace(pass == RenderPass::Shadow ? GL_FRONT : GL_BACK));

	// Attach instance buffer and render
	m_vertexArray.bind();
	m_vertexArray.addBuffer(m_instanceBuffer, 1, 4, sizeof(Vector4f), offset, 1);
	m_vertexArray.draw(renderList.size());
}


///////////////////////////////////////////////////////////
Image* Terrain::createNormalTile(Image* hmap, float chunkSize)
{
	// Create a new image
	Image* normalImg = Pool<Image>::alloc();
	normalImg->create(0, hmap->getWidth(), hmap->getHeight(), 3, GLType::Uint16);

	// Calculate normals
	Vector3<Uint16>* normals = (Vector3<Uint16>*)normalImg->getData();
	Vector2u mapSize(hmap->getWidth(), hmap->getHeight());
	Vector2f sizeFactor = chunkSize / Vector2f(mapSize);

	for (Uint32 r = 0; r < mapSize.y; ++r)
	{
		for (Uint32 c = 0; c < mapSize.x; ++c)
		{
			// Calculate normal
			float h01 = *(float*)hmap->getPixel(r, c - (c == 0 ? 0 : 1)) * m_maxHeight;
			float h21 = *(float*)hmap->getPixel(r, c + (c == mapSize.x - 1 ? 0 : 1)) * m_maxHeight;
			float h10 = *(float*)hmap->getPixel(r + (r == mapSize.y - 1 ? 0 : 1), c) * m_maxHeight;
			float h12 = *(float*)hmap->getPixel(r - (r == 0 ? 0 : 1), c) * m_maxHeight;

			Vector3f v1(sizeFactor.x, h21 - h01, 0.0f);
			Vector3f v2(0.0f, h12 - h10, -sizeFactor.y);
			Vector3f normal = normalize(cross(v1, v2));

			// Adjust for 16-bit
			normal.x = 0.5f * normal.x + 0.5f;
			normal.z = 0.5f * normal.z + 0.5f;

			normals[r * mapSize.x + c] = Vector3<Uint16>(normal * 65535.0f);
		}
	}

	// Free height map (for now)
	Pool<Image>::free(hmap);

	return normalImg;
}


///////////////////////////////////////////////////////////
bool intersects(const Vector3f& p, float r, const BoundingBox& bbox)
{
	float dmin = 0.0f;
	if (p.x < bbox.m_min.x)
		dmin += powf(p.x - bbox.m_min.x, 2.0f);
	else if (p.x > bbox.m_max.x)
		dmin += powf(p.x - bbox.m_max.x, 2.0f);

	if (p.y < bbox.m_min.y)
		dmin += powf(p.y - bbox.m_min.y, 2.0f);
	else if (p.y > bbox.m_max.y)
		dmin += powf(p.y - bbox.m_max.y, 2.0f);

	if (p.z < bbox.m_min.z)
		dmin += powf(p.z - bbox.m_min.z, 2.0f);
	else if (p.z > bbox.m_max.z)
		dmin += powf(p.z - bbox.m_max.z, 2.0f);

	return dmin <= r * r;
}


///////////////////////////////////////////////////////////
void Terrain::addLodNodes(const Vector2u& node, Uint32 lod, const Frustum& frustum, std::vector<Vector4f>& renderList)
{
	// Calculate node properties
	Uint32 numNodesPerEdge = 1 << lod;
	float nodeSize = m_size / (float)numNodesPerEdge;
	float halfNodeSize = 0.5f * nodeSize;
	Vector2f center = nodeSize * (Vector2f(node.y, node.x) - (float)(numNodesPerEdge / 2) + 0.5f);
	if (lod == 0)
		center = Vector2f(0.0f);

	// Get bounding box
	const Vector2<Uint16>& hbounds = m_lodLevels[lod].m_heightBounds[node.x * numNodesPerEdge + node.y];

	BoundingBox bbox;
	bbox.m_min = Vector3f(center.x - halfNodeSize, (float)hbounds.x / 65535.0f * m_maxHeight, center.y - halfNodeSize);
	bbox.m_max = Vector3f(center.x + halfNodeSize, (float)hbounds.y / 65535.0f * m_maxHeight, center.y + halfNodeSize);

	// Check if bounding box is inside frustum
	if (!frustum.contains(bbox)) return;

	// Check if bounding box is inside the current lod range
	if (intersects(m_viewpoint, m_lodLevels[lod].m_range, bbox))
	{
		// Check if this node is inside a more detailed lod level
		if (lod == m_lodLevels.size() - 1 || !intersects(m_viewpoint, m_lodLevels[lod + 1].m_range, bbox))
		{
			// If it's not, then add entire node at the current lod level
			float fourthSize = 0.25f * nodeSize;
			float scale = (float)(1 << (m_lodLevels.size() - lod - 1));
			renderList.push_back(Vector4f(center + Vector2f(-fourthSize, -fourthSize), scale * m_baseScale, (float)lod + 0.5f));
			renderList.push_back(Vector4f(center + Vector2f(-fourthSize, fourthSize), scale * m_baseScale, (float)lod + 0.5f));
			renderList.push_back(Vector4f(center + Vector2f(fourthSize, -fourthSize), scale * m_baseScale, (float)lod + 0.5f));
			renderList.push_back(Vector4f(center + Vector2f(fourthSize, fourthSize), scale * m_baseScale, (float)lod + 0.5f));
		}
		else
		{
			// If current node is inside a more detailed lod level, add children nodes
			Vector2u childNode = 2u * node;
			addLodNodes(childNode + Vector2u(0, 0), lod + 1, frustum, renderList);
			addLodNodes(childNode + Vector2u(0, 1), lod + 1, frustum, renderList);
			addLodNodes(childNode + Vector2u(1, 0), lod + 1, frustum, renderList);
			addLodNodes(childNode + Vector2u(1, 1), lod + 1, frustum, renderList);
		}
	}
	else
	{
		// If this node is outside the lod range, this means the parent
		// node was inside its own range, but this child node is not
		// In this case, add the current node, but don't attempt to add children nodes
		--lod;
		float scale = (float)(1 << (m_lodLevels.size() - lod - 1));
		renderList.push_back(Vector4f(center, scale * m_baseScale, (float)lod + 0.5f));

		return;
	}
}


///////////////////////////////////////////////////////////
void Terrain::loadMapTiles(const Vector2u& node, Uint32 lod)
{
	// Maps
	TerrainMap* hmap = TMAP_CAST(m_heightMap);
	TerrainMap* nmap = TMAP_CAST(m_normalMap);

	// Calculate node properties
	Uint32 numNodesPerEdge = 1 << lod;
	float nodeSize = m_size / (float)numNodesPerEdge;
	float halfNodeSize = 0.5f * nodeSize;
	Vector2f center = nodeSize * (Vector2f(node.y, node.x) - (float)(numNodesPerEdge / 2) + 0.5f);
	if (lod == 0)
		center = Vector2f(0.0f);

	// Get bounding box
	const Vector2<Uint16>& hbounds = m_lodLevels[lod].m_heightBounds[node.x * numNodesPerEdge + node.y];

	BoundingBox bbox;
	bbox.m_min = Vector3f(center.x - halfNodeSize, (float)hbounds.x / 65535.0f * m_maxHeight, center.y - halfNodeSize);
	bbox.m_max = Vector3f(center.x + halfNodeSize, (float)hbounds.y / 65535.0f * m_maxHeight, center.y + halfNodeSize);

	// Check if bounding box is inside the current lod range
	Vector3<Uint8> nodeData(node.x, node.y, lod);
	if (intersects(m_viewpoint, m_lodLevels[lod + 1].m_range, bbox))
	{
		// Check if the tile is loaded yet
		if (m_loadedMapTiles.find(nodeData) == m_loadedMapTiles.end())
		{
			// Get next free index
			Vector2<Uint8> nextFree = m_indexFreeList.top();
			m_indexFreeList.pop();

			// Load tiles (transform to xy instead of rc)
			Vector2i tile = Vector2i(node.y, node.x) - (int)numNodesPerEdge / 2;

			// Height map
			{
				AsyncTileObj asyncObj;
				asyncObj.m_future = std::async(std::launch::async, &TerrainMap::load, hmap, tile, lod);
				asyncObj.m_cacheTexture = &hmap->m_texture;
				asyncObj.m_nodeData = nodeData;
				asyncObj.m_cachePos = nextFree;
				m_loadThreads.push_back(std::move(asyncObj));
			}
		}

		// Quit if higher than the highest map level
		if (lod >= m_highestMapLevel) return;

		// Load maps for children
		Vector2u childNode = 2u * node;
		loadMapTiles(childNode + Vector2u(0, 0), lod + 1);
		loadMapTiles(childNode + Vector2u(0, 1), lod + 1);
		loadMapTiles(childNode + Vector2u(1, 0), lod + 1);
		loadMapTiles(childNode + Vector2u(1, 1), lod + 1);
	}

	// Don't remove base tile
	else if (lod > 0)
	{
		// Check if the tile needs to be unloaded
		auto it = m_loadedMapTiles.find(nodeData);
		if (it != m_loadedMapTiles.end())
		{
			// Add the tile coords to the free list
			Vector2<Uint8> redirectLoc = it.value();
			m_indexFreeList.push(redirectLoc);

			// Update redirect locations to point at tile at lower lod
			Vector3<Uint8> parentNode = Vector3<Uint8>(node / 2u, lod - 1);
			auto parentTileEntry = m_loadedMapTiles.find(parentNode);
			ASSERT(parentTileEntry != m_loadedMapTiles.end(), "Parent node map tile hasn't been loaded (Terrain)");

			// Get new redirect location
			Vector2<Uint8> newRedirectLoc = parentTileEntry.value();

			// Get bounds of tile in the index map
			Uint32 pixelsPerTile = 1 << (m_highestMapLevel - lod);
			Uint32 sr = node.x * pixelsPerTile;
			Uint32 sc = node.y * pixelsPerTile;
			Uint32 fr = (node.x + 1) * pixelsPerTile;
			Uint32 fc = (node.y + 1) * pixelsPerTile;

			for (Uint32 r = sr; r < fr; ++r)
			{
				for (Uint32 c = sc; c < fc; ++c)
					m_indexMapImg[r][c] = newRedirectLoc;
			}

			// Indicate that the index map has changed
			m_indexMapChanged = true;

			// Remove from map
			m_loadedMapTiles.erase(it);
		}
	}
}


///////////////////////////////////////////////////////////
void Terrain::setViewpoint(const Vector3f& point)
{
	m_viewpointChanged = m_viewpoint != point;
	m_viewpoint = point;
	m_useCustomViewpoint = true;
}


///////////////////////////////////////////////////////////
void Terrain::setHeightMap(const Image& hmap)
{
	// Make sure image has correct requirements
	ASSERT(hmap.getWidth() == hmap.getHeight() && hmap.getWidth() != 0, "Height maps must be square with a size greater than 0");
	ASSERT((hmap.getWidth() & (hmap.getWidth() - 1)) == 0, "Height map size must be a power of two");
	ASSERT(hmap.getDataType() == GLType::Float || hmap.getDataType() == GLType::Uint16, "Height maps must contain data with either the Float or Uint16 type");
	ASSERT(hmap.getNumChannels() == 1, "Height maps must contain a single color channel");

	// Create image
	if (hmap.getDataType() == GLType::Float)
		m_heightMapImg.create(hmap.getData(), hmap.getWidth(), hmap.getHeight(), 1, GLType::Float, false);

	else
	{
		// Convert 16-bit to float
		Uint32 numPixels = hmap.getWidth() * hmap.getHeight();
		float* dst = (float*)malloc(numPixels * sizeof(float));
		Uint16* src = (Uint16*)hmap.getData();

		for (Uint32 i = 0; i < numPixels; ++i)
			dst[i] = (float)src[i] / 65535.0f;

		m_heightMapImg.create(dst, hmap.getWidth(), hmap.getHeight(), 1, GLType::Float, true);
	}

	// Create texture
	if (!m_heightMap)
		m_heightMap = Pool<Texture>::alloc();
	TEXTURE_CAST(m_heightMap)->create(m_heightMapImg);

	// Update entire map
	updateData(Vector2u(0), Vector2u(hmap.getWidth(), hmap.getHeight()));

	// Indicate that normal images are used
	m_usesTerrainMaps = false;
}


///////////////////////////////////////////////////////////
void Terrain::setHeightMap(TerrainMap* hmap)
{
	// Make sure image has correct requirements
	ASSERT((hmap->getSize() & (hmap->getSize() - 1)) == 0, "Height map size must be a power of two");
	ASSERT(hmap->getDataType() == GLType::Float, "Height maps must contain data with either the Float type");
	ASSERT(hmap->getFormat() == PixelFormat::R, "Height maps must contain a single color channel");

	// Set tile size
	if (!m_tileSize)
		m_tileSize = hmap->getTileSize();
	ASSERT(m_tileSize == hmap->getTileSize(), "Tile size must match previously set tile size");

	// Create index map
	createIndexMap(hmap->getSize());

	// Set terrain map
	m_heightMap = hmap;

	// Set cache size
	Vector2u cacheSize = estimateCacheSize(m_highestMapLevel + 1);
	hmap->setCacheSize(cacheSize.x, cacheSize.y);

	// Indicate that terrain maps are used
	m_usesTerrainMaps = true;
}


///////////////////////////////////////////////////////////
void Terrain::createIndexMap(Uint32 size)
{
	// Don't create twice
	if (m_indexMap.getId()) return;

	// Calculate highest tile level
	Uint32 currSize = size;
	while (currSize > m_tileSize)
	{
		currSize /= 2;
		++m_highestMapLevel;
	}

	// Create free list
	Vector2i cacheSize = estimateCacheSize(m_highestMapLevel + 1);
	for (int r = cacheSize.y - 1; r >= 0; --r)
	{
		if (r < cacheSize.y - 1)
			m_indexFreeList.push(Vector2<Uint8>(r + 1, 0));

		for (int c = cacheSize.x - 2; c >= 0; --c)
			m_indexFreeList.push(Vector2<Uint8>(r, c + 1));
	}
	m_indexFreeList.push(Vector2<Uint8>(0));

	// Create index image
	int mapSize = 1 << m_highestMapLevel;
	m_indexMapImg.create(mapSize, mapSize, Vector2<Uint8>(0));

	// Create texture
	m_indexMap.create(m_indexMapImg.getData(), PixelFormat::Rg, mapSize, mapSize, 0, GLType::Uint8, TextureFilter::Nearest);

	// Mark changed flag
	m_indexMapChanged = true;
}


///////////////////////////////////////////////////////////
void Terrain::updateData(const Vector2u& pos, const Vector2u& size)
{
	// Calculate rectangle in node coordinates
	Uint32 numTilesPerEdge = 1 << (m_lodLevels.size() - 1);
	Uint32 pixelsPerTile = TEXTURE_CAST(m_heightMap)->getWidth() / numTilesPerEdge;
	Uint32 sr = pos.y / pixelsPerTile;
	Uint32 sc = pos.x / pixelsPerTile;
	Uint32 fr = (pos.y + size.y - 1) / pixelsPerTile;
	Uint32 fc = (pos.x + size.x - 1) / pixelsPerTile;

	// Reset height bounds
	for (int i = m_lodLevels.size() - 1; i >= 0; --i)
	{
		LodLevel& level = m_lodLevels[i];

		Uint32 denom = (Uint32)(1 << (m_lodLevels.size() - i - 1));
		Vector2u start = Vector2u(sr, sc) / denom;
		Vector2u finish = Vector2u(fr, fc) / denom;
		Uint32 tilesPerEdge = numTilesPerEdge / denom;

		// Reset bounds in the region
		for (Uint32 r = start.x; r <= finish.x; ++r)
		{
			for (Uint32 c = start.y; c <= finish.y; ++c)
			{
				Uint32 index = r * tilesPerEdge + c;
				level.m_heightBounds[index] = Vector2<Uint16>(65535, 0);
			}
		}
	}

	// Update height bounds
	for (Uint32 r = sr; r <= fr; ++r)
	{
		for (Uint32 c = sc; c <= fc; ++c)
			updateHeightBounds(r, c);
	}

	// Normal map
	Vector2u mapSize = Vector2u(m_heightMapImg.getWidth(), m_heightMapImg.getHeight());
	Vector2f sizeFactor = m_size / Vector2f(mapSize);

	// If a normal map hasn't been created, create a new one
	if (!m_normalMap)
	{
		// Create empty image
		m_normalMapImg.create(0, mapSize.x, mapSize.y, 3, GLType::Uint16);

		// Create texture
		Texture* nmap = Pool<Texture>::alloc();
		nmap->create(m_normalMapImg);
		m_normalMap = nmap;
	}

	// Calculate normals
	Vector3<Uint16>* normals = (Vector3<Uint16>*)m_normalMapImg.getData();
	sr = pos.x;
	sc = pos.y;
	fr = pos.x + size.x;
	fc = pos.y + size.y;

	for (Uint32 r = sr; r < fr; ++r)
	{
		for (Uint32 c = sc; c < fc; ++c)
		{
			// Calculate normal
			float h01 = *(float*)m_heightMapImg.getPixel(r, c - (c == 0 ? 0 : 1)) * m_maxHeight;
			float h21 = *(float*)m_heightMapImg.getPixel(r, c + (c == mapSize.x - 1 ? 0 : 1)) * m_maxHeight;
			float h10 = *(float*)m_heightMapImg.getPixel(r + (r == mapSize.y - 1 ? 0 : 1), c) * m_maxHeight;
			float h12 = *(float*)m_heightMapImg.getPixel(r - (r == 0 ? 0 : 1), c) * m_maxHeight;

			Vector3f v1(sizeFactor.x, h21 - h01, 0.0f);
			Vector3f v2(0.0f, h12 - h10, -sizeFactor.y);
			Vector3f normal = normalize(cross(v1, v2));

			// Adjust for 16-bit
			normal.x = 0.5f * normal.x + 0.5f;
			normal.z = 0.5f * normal.z + 0.5f;

			normals[r * mapSize.x + c] = Vector3<Uint16>(normal * 65535.0f);
		}
	}

	// Update texture
	TEXTURE_CAST(m_normalMap)->update(normals);
}


///////////////////////////////////////////////////////////
void Terrain::updateHeightBounds(Uint32 nr, Uint32 nc)
{
	// Calculate node bounds in pixels
	Uint32 numTilesPerEdge = 1 << (m_lodLevels.size() - 1);
	Uint32 pixelsPerTile = TEXTURE_CAST(m_heightMap)->getWidth() / numTilesPerEdge;
	Uint32 sr = (nr + 0) * pixelsPerTile;
	Uint32 sc = (nc + 0) * pixelsPerTile;
	Uint32 fr = (nr + 1) * pixelsPerTile;
	Uint32 fc = (nc + 1) * pixelsPerTile;

	Uint16 min = 65535;
	Uint16 max = 0;

	// Find height bounds
	for (Uint32 r = sr; r < fr; ++r)
	{
		for (Uint32 c = sc; c < fc; ++c)
		{
			// Get height value
			Uint16 value = (Uint16)(*(float*)m_heightMapImg.getPixel(r, c) * 65535.0f);

			if (value < min)
				min = value;
			if (value > max)
				max = value;
		}
	}

	// Update height bounds for all nodes up to root
	bool boundsChanged = true;
	int level = m_lodLevels.size() - 1;
	Vector2<Uint16> newBounds = Vector2<Uint16>(min, max);

	while (boundsChanged && level >= 0)
	{
		// Reset flag to false
		boundsChanged = false;

		// Get current bounds
		Vector2<Uint16>& currBounds = m_lodLevels[level].m_heightBounds[nr * numTilesPerEdge + nc];

		if (newBounds.x < currBounds.x)
		{
			currBounds.x = newBounds.x;
			boundsChanged = true;
		}
		if (newBounds.y > currBounds.y)
		{
			currBounds.y = newBounds.y;
			boundsChanged = true;
		}

		// Update state
		--level;
		nr /= 2;
		nc /= 2;
		numTilesPerEdge /= 2;
	}
}


///////////////////////////////////////////////////////////
void Terrain::setHeightBounds(const Vector2i& tile, const Vector2f& bounds)
{
	Uint32 numTilesPerEdge = 1 << (m_lodLevels.size() - 1);

	// Transform tile coords to indices
	Vector2u indices = tile + (int)numTilesPerEdge / 2;

	// Update height bounds for all nodes up to root
	bool boundsChanged = true;
	int level = m_lodLevels.size() - 1;
	Vector2<Uint16> newBounds = Vector2<Uint16>(bounds * 65535.0f);

	while (boundsChanged && level >= 0)
	{
		// Reset flag to false
		boundsChanged = false;

		// Get current bounds
		Vector2<Uint16>& currBounds = m_lodLevels[level].m_heightBounds[indices.y * numTilesPerEdge + indices.x];

		if (newBounds.x < currBounds.x)
		{
			currBounds.x = newBounds.x;
			boundsChanged = true;
		}
		if (newBounds.y > currBounds.y)
		{
			currBounds.y = newBounds.y;
			boundsChanged = true;
		}

		// Update state
		--level;
		indices.y /= 2;
		indices.x /= 2;
		numTilesPerEdge /= 2;
	}
}


}