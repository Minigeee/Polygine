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
Vector2u estimateCacheSize(Uint32 numLevels)
{
	// Get the number of required tiles in the very worst case (this will be the size of cache)
	Uint32 numTiles = 1;
	numTiles += numLevels >= 2 ? 4 : 0;
	numTiles += numLevels >= 3 ? 9 * (numLevels - 2) : 0;

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
void copyPixel(Image* src, Image* dst, const Vector2f& srcUv, const Vector2u& dstRc)
{
	GLType dtype = src->getDataType();
	Uint32 c = src->getNumChannels();

	if (c == 1)
	{
		if (dtype == GLType::Uint8)
			*(Uint8*)dst->getPixel(dstRc.x, dstRc.y) = src->sample<Uint8>(srcUv);
		else if (dtype == GLType::Uint16)
			*(Uint16*)dst->getPixel(dstRc.x, dstRc.y) = src->sample<Uint16>(srcUv);
		else if (dtype == GLType::Float)
			*(float*)dst->getPixel(dstRc.x, dstRc.y) = src->sample<float>(srcUv);
	}
	else if (c == 2)
	{
		if (dtype == GLType::Uint8)
			*(Vector2<Uint8>*)dst->getPixel(dstRc.x, dstRc.y) = src->sample<Vector2<Uint8>>(srcUv);
		else if (dtype == GLType::Uint16)
			*(Vector2<Uint16>*)dst->getPixel(dstRc.x, dstRc.y) = src->sample<Vector2<Uint16>>(srcUv);
		else if (dtype == GLType::Float)
			*(Vector2f*)dst->getPixel(dstRc.x, dstRc.y) = src->sample<Vector2f>(srcUv);
	}
	else if (c == 3)
	{
		if (dtype == GLType::Uint8)
			*(Vector3<Uint8>*)dst->getPixel(dstRc.x, dstRc.y) = src->sample<Vector3<Uint8>>(srcUv);
		else if (dtype == GLType::Uint16)
			*(Vector3<Uint16>*)dst->getPixel(dstRc.x, dstRc.y) = src->sample<Vector3<Uint16>>(srcUv);
		else if (dtype == GLType::Float)
			*(Vector3f*)dst->getPixel(dstRc.x, dstRc.y) = src->sample<Vector3f>(srcUv);
	}
	else if (c == 4)
	{
		if (dtype == GLType::Uint8)
			*(Vector4<Uint8>*)dst->getPixel(dstRc.x, dstRc.y) = src->sample<Vector4<Uint8>>(srcUv);
		else if (dtype == GLType::Uint16)
			*(Vector4<Uint16>*)dst->getPixel(dstRc.x, dstRc.y) = src->sample<Vector4<Uint16>>(srcUv);
		else if (dtype == GLType::Float)
			*(Vector4f*)dst->getPixel(dstRc.x, dstRc.y) = src->sample<Vector4f>(srcUv);
	}
}


///////////////////////////////////////////////////////////
void fillCorner(Image* img, const Vector2u& cornerRc, const Vector2u& adj1Rc, const Vector2u& adj2Rc)
{
	GLType dtype = img->getDataType();
	Uint32 c = img->getNumChannels();

	if (dtype == GLType::Uint8)
	{
		Uint8* corner = (Uint8*)img->getPixel(cornerRc.x, cornerRc.y);
		Uint8* adj1 = (Uint8*)img->getPixel(adj1Rc.x, adj1Rc.y);
		Uint8* adj2 = (Uint8*)img->getPixel(adj2Rc.x, adj2Rc.y);

		// Calculate averages
		for (Uint32 i = 0; i < c; ++i)
			corner[i] = (Uint8)(((float)adj1[i] + (float)adj2[i]) * 0.5f);
	}
	else if (dtype == GLType::Uint16)
	{
		Uint16* corner = (Uint16*)img->getPixel(cornerRc.x, cornerRc.y);
		Uint16* adj1 = (Uint16*)img->getPixel(adj1Rc.x, adj1Rc.y);
		Uint16* adj2 = (Uint16*)img->getPixel(adj2Rc.x, adj2Rc.y);

		// Calculate averages
		for (Uint32 i = 0; i < c; ++i)
			corner[i] = (Uint16)(((float)adj1[i] + (float)adj2[i]) * 0.5f);
	}
	else if (dtype == GLType::Float)
	{
		float* corner = (float*)img->getPixel(cornerRc.x, cornerRc.y);
		float* adj1 = (float*)img->getPixel(adj1Rc.x, adj1Rc.y);
		float* adj2 = (float*)img->getPixel(adj2Rc.x, adj2Rc.y);

		// Calculate averages
		for (Uint32 i = 0; i < c; ++i)
			corner[i] = ((float)adj1[i] + (float)adj2[i]) * 0.5f;
	}
}


///////////////////////////////////////////////////////////
TerrainBase::TerrainBase() :
	m_size					(0.0f),
	m_maxHeight				(0.0f),
	m_numLevels				(0),
	m_viewpoint				(0.0f),
	m_baseScale				(1.0f),
	m_shader				(0),
	m_instanceDataOffset	(0),
	m_viewpointChanged		(false),
	m_lodDistsChanged		(false)
{

}


///////////////////////////////////////////////////////////
TerrainBase::~TerrainBase()
{

}


///////////////////////////////////////////////////////////
void TerrainBase::init(Scene* scene)
{
	m_scene = scene;
}


///////////////////////////////////////////////////////////
void TerrainBase::create(float size, float maxHeight, float maxNodeSize)
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

		// Add lod level
		m_lodLevels.push_back(LodLevel());
		m_lodLevels.back().m_heightBounds.create(numNodes, Vector2<Uint16>(0, (Uint16)(m_maxHeight * 65535.0f)));

		// Update size
		currSize *= 0.5f;
	}

	// Calculate lod distances
	float distUnit = 2.0f * currSize;
	float prevDist = distUnit;

	for (int i = m_lodLevels.size() - 1; i >= 0; --i)
	{
		m_lodLevels[i].m_dist = prevDist;
		prevDist += powf(2.0f, (float)(m_lodLevels.size() - i)) * distUnit;
	}

	m_lodDistsChanged = true;

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
void TerrainBase::render(Camera& camera, RenderPass pass, const RenderSettings& settings)
{
	// The terrain should be rendered regardless of render pass

	ASSERT(m_scene, "The terrain must be initialized before using, by calling the init() function");

	START_PROFILING_FUNC;


	// Update view point if using default render pass
	if (pass == RenderPass::Default)
	{
		// Set flag if viewpoint changes
		if (m_viewpoint != camera.getPosition())
			m_viewpointChanged = true;

		m_viewpoint = camera.getPosition();
	}

	// Get lod nodes
	std::vector<Vector4f> renderList;
	makeRenderList(Vector2u(0), 0, camera.getFrustum(), renderList);

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


	// Custom render procedure
	onRender(camera);

	// Terrain
	m_shader->setUniform("u_size", m_size);
	m_shader->setUniform("u_maxHeight", m_maxHeight);

	if (m_lodDistsChanged)
	{
		// Set lod distances
		for (Uint32 i = 0; i < m_lodLevels.size(); ++i)
			m_shader->setUniform("u_lodRanges[" + std::to_string(i) + ']', m_lodLevels[i].m_dist);
		m_shader->setUniform("u_lodRanges[" + std::to_string(m_lodLevels.size()) + ']', 0.0f);

		m_lodDistsChanged = false;
	}

	// Camera
	camera.apply(m_shader);

	// Enable depth testing
	glCheck(glEnable(GL_DEPTH_TEST));

	// Single side render
	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glCullFace(pass == RenderPass::Shadow ? GL_FRONT : GL_BACK));

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Attach instance buffer and render
	m_vertexArray.bind();
	m_vertexArray.addBuffer(m_instanceBuffer, 1, 4, sizeof(Vector4f), offset, 1);
	m_vertexArray.draw(renderList.size());

	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


///////////////////////////////////////////////////////////
void TerrainBase::makeRenderList(const Vector2u& node, Uint32 lod, const Frustum& frustum, std::vector<Vector4f>& renderList)
{
	// Calculate node properties
	Uint32 numNodesPerEdge = 1 << lod;
	float nodeSize = m_size / (float)numNodesPerEdge;
	float halfNodeSize = 0.5f * nodeSize;
	Vector2f center = nodeSize * (Vector2f(node.y, node.x) - (float)(numNodesPerEdge / 2) + 0.5f);
	if (lod == 0)
		center = Vector2f(0.0f);

	// Get bounding box
	BoundingBox bbox;
	{
		std::unique_lock<std::mutex> lock(m_mutex);

		const Vector2<Uint16>& hbounds = m_lodLevels[lod].m_heightBounds[node.x][node.y];

		bbox.m_min = Vector3f(center.x - halfNodeSize, (float)hbounds.x / 65535.0f * m_maxHeight, center.y - halfNodeSize);
		bbox.m_max = Vector3f(center.x + halfNodeSize, (float)hbounds.y / 65535.0f * m_maxHeight, center.y + halfNodeSize);
	}

	// Check if bounding box is inside frustum
	if (!frustum.contains(bbox)) return;

	// Check if bounding box is inside the current lod range
	if (intersects(m_viewpoint, m_lodLevels[lod].m_dist, bbox))
	{
		// Check if this node is inside a more detailed lod level
		if (lod == m_lodLevels.size() - 1 || !intersects(m_viewpoint, m_lodLevels[lod + 1].m_dist, bbox))
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
			makeRenderList(childNode + Vector2u(0, 0), lod + 1, frustum, renderList);
			makeRenderList(childNode + Vector2u(0, 1), lod + 1, frustum, renderList);
			makeRenderList(childNode + Vector2u(1, 0), lod + 1, frustum, renderList);
			makeRenderList(childNode + Vector2u(1, 1), lod + 1, frustum, renderList);
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
Terrain::Terrain()
{

}


///////////////////////////////////////////////////////////
Terrain::~Terrain()
{

}


///////////////////////////////////////////////////////////
void Terrain::onRender(Camera& camera)
{
	// Get shader
	if (!m_shader)
		m_shader = &getShader();

	// Bind shader
	m_shader->bind();

	// Bind textures
	m_shader->setUniform("u_heightMap", m_heightMap);
	m_shader->setUniform("u_normalMap", m_normalMap);
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
		m_heightMapImg = hmap.getBuffer<float>();

	else
		// Convert 16-bit to float
		m_heightMapImg = ImageBuffer<float>(hmap.getBuffer<Uint16>()) / 65535.0f;

	// Create texture
	m_heightMap.create(NULL, PixelFormat::R, hmap.getWidth(), hmap.getHeight(), 0, GLType::Float);

	// Update entire map
	updateHeightMap(Vector2u(0), Vector2u(hmap.getWidth(), hmap.getHeight()));
}


///////////////////////////////////////////////////////////
void Terrain::updateHeightMap(const Vector2u& pos, const Vector2u& size)
{
	// Check if full image is being updated
	bool fullImgUpdate = pos == Vector2u(0) && size.x == m_heightMap.getWidth() && size.x == m_heightMap.getHeight();

	// Calculate rectangle in node coordinates
	Uint32 numTilesPerEdge = 1 << (m_lodLevels.size() - 1);
	Uint32 pixelsPerTile = m_heightMap.getWidth() / numTilesPerEdge;
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
				level.m_heightBounds[r][c] = Vector2<Uint16>(65535, 0);
		}
	}

	// Update height bounds
	for (Uint32 r = sr; r <= fr; ++r)
	{
		for (Uint32 c = sc; c <= fc; ++c)
			updateHeightBounds(r, c);
	}

	// Update height map
	if (fullImgUpdate)
		m_heightMap.update(m_heightMapImg.getData());

	else
	{
		Uint32 numPixels = size.x * size.y;
		float* dst = (float*)malloc(numPixels * sizeof(float));
		float* src = (float*)m_heightMapImg.getData();

		// Copy row by row
		for (Uint32 r = 0; r < size.y; ++r)
			memcpy(dst + r * size.x, src + (pos.x + r) * m_heightMapImg.getWidth() + pos.y, size.x * sizeof(float));

		// Update texture
		m_heightMap.update(dst, Vector2u(pos.y, pos.x), size);

		// Free data
		free(dst);
	}

	// Normal map
	Vector2u mapSize = Vector2u(m_heightMapImg.getWidth(), m_heightMapImg.getHeight());
	Vector2f sizeFactor = m_size / Vector2f(mapSize);

	// If a normal map hasn't been created, create a new one
	if (!m_normalMap.getId())
	{
		// Create empty image
		m_normalMapImg.create(mapSize.x, mapSize.y);

		// Create texture
		m_normalMap.create(NULL, PixelFormat::Rgb, mapSize.x, mapSize.y, 0, GLType::Uint16);
	}

	// Calculate normals
	sr = pos.x;
	sc = pos.y;
	fr = pos.x + size.x;
	fc = pos.y + size.y;

	for (Uint32 r = sr; r < fr; ++r)
	{
		for (Uint32 c = sc; c < fc; ++c)
		{
			// Calculate normal
			float h01 = m_heightMapImg[r][c - (c == 0 ? 0 : 1)] * m_maxHeight;
			float h21 = m_heightMapImg[r][c + (c == mapSize.x - 1 ? 0 : 1)] * m_maxHeight;
			float h10 = m_heightMapImg[r + (r == mapSize.y - 1 ? 0 : 1)][c] * m_maxHeight;
			float h12 = m_heightMapImg[r - (r == 0 ? 0 : 1)][c] * m_maxHeight;

			Vector3f v1(sizeFactor.x, h21 - h01, 0.0f);
			Vector3f v2(0.0f, h12 - h10, -sizeFactor.y);
			Vector3f normal = normalize(cross(v1, v2));

			// Adjust for 16-bit
			normal.x = 0.5f * normal.x + 0.5f;
			normal.z = 0.5f * normal.z + 0.5f;

			m_normalMapImg[r][c] = Vector3<Uint16>(normal * 65535.0f);
		}
	}

	// Update normal map
	if (fullImgUpdate)
		m_normalMap.update(m_normalMapImg.getData());

	else
	{
		Uint32 numPixels = size.x * size.y;
		Vector3<Uint16>* dst = (Vector3<Uint16>*)malloc(numPixels * sizeof(Vector3<Uint16>));
		Vector3<Uint16>* src = (Vector3<Uint16>*)m_normalMapImg.getData();

		// Copy row by row
		for (Uint32 r = 0; r < size.y; ++r)
			memcpy(dst + r * size.x, src + (pos.x + r) * m_normalMapImg.getWidth() + pos.y, size.x * sizeof(Vector3<Uint16>));

		// Update texture
		m_normalMap.update(dst, Vector2u(pos.y, pos.x), size);

		// Free data
		free(dst);
	}
}


///////////////////////////////////////////////////////////
void Terrain::updateHeightBounds(Uint32 nr, Uint32 nc)
{
	// Calculate node bounds in pixels
	Uint32 numTilesPerEdge = 1 << (m_lodLevels.size() - 1);
	Uint32 pixelsPerTile = m_heightMap.getWidth() / numTilesPerEdge;
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
			Uint16 value = (Uint16)(m_heightMapImg[r][c] * 65535.0f);

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
		Vector2<Uint16>& currBounds = m_lodLevels[level].m_heightBounds[nr][nc];

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
Texture& Terrain::getHeightMap()
{
	return m_heightMap;
}


///////////////////////////////////////////////////////////
Texture& Terrain::getNormalMap()
{
	return m_normalMap;
}


///////////////////////////////////////////////////////////
Terrain::HeightMap& Terrain::getHeightData()
{
	return m_heightMapImg;
}


///////////////////////////////////////////////////////////
Terrain::NormalMap& Terrain::getNormalData()
{
	return m_normalMapImg;
}


///////////////////////////////////////////////////////////
Shader LargeTerrain::s_shader;


///////////////////////////////////////////////////////////
Shader& LargeTerrain::getShader()
{
	if (!s_shader.getId())
	{
		s_shader.load("shaders/large_terrain.vert", Shader::Vertex);
		s_shader.load("shaders/large_terrain.frag", Shader::Fragment);
		s_shader.compile();
	}

	return s_shader;
}


///////////////////////////////////////////////////////////
LargeTerrain::LargeTerrain() :
	m_tileSize				(512.0f),
	m_baseTileLevel			(0),
	m_cacheMapSize			(0),
	m_tileLoadedBitfield	(0),
	m_redirectMapChanged	(false)
{
	// Need to do initial load
	m_viewpointChanged = true;
}


///////////////////////////////////////////////////////////
LargeTerrain::~LargeTerrain()
{
	// Free all images and edge images
	for (auto it = m_tileMap.begin(); it != m_tileMap.end(); ++it)
	{
		for (Uint32 i = 0; i < it.value().m_mapData.size(); ++i)
		{
			MapData& data = it.value().m_mapData[i];

			if (data.m_fullImg)
				Pool<Image>::free(data.m_fullImg);
			if (data.m_edgeImg)
				Pool<Image>::free(data.m_edgeImg);

			data.m_fullImg = 0;
			data.m_edgeImg = 0;
		}
	}

	// Delete all load tasks
	for (Uint32 i = 0; i < m_loadTasks.size(); ++i)
		delete m_loadTasks[i];

	m_loadTasks.clear();
}


///////////////////////////////////////////////////////////
void LargeTerrain::create(float size, float maxHeight, float maxBaseSize, float tileSize)
{
	// Standard create routine
	TerrainBase::create(size, maxHeight, maxBaseSize);

	// Tile properties
	m_tileSize = tileSize;

	// Round tile size to nearest lod level size
	float baseSize = m_lodLevels.back().m_dist;
	m_baseTileLevel = (Uint32)std::max((int)std::lround(std::log2f(m_tileSize / baseSize)), 0);
	m_tileSize = (float)(1 << m_baseTileLevel) * baseSize;
	m_baseTileLevel = m_lodLevels.size() - m_baseTileLevel - 1;

	// Create redirect map
	Uint32 mapSize = 1 << m_baseTileLevel;
	m_redirectMapImg.create(mapSize);

	// Create texture (nearest filter)
	m_redirectMap.create(m_redirectMapImg.getData(), PixelFormat::Rgb, mapSize, mapSize, 0, GLType::Uint8, TextureFilter::Nearest);

	// Estimate cache map size
	m_cacheMapSize = estimateCacheSize(m_baseTileLevel + 1);

	// Create free list (add in reverse)
	for (int y = m_cacheMapSize.y - 1; y >= 0; --y)
	{
		for (int x = m_cacheMapSize.x - 1; x >= 0; --x)
			m_freeList.push(Vector2<Uint8>(x, y));
	}

	// The scheduler is used, so initialize it if it has not
	if (!Scheduler::getNumWorkers())
		Scheduler::setNumWorkers(std::max((int)std::thread::hardware_concurrency() - 1, 1));
}


///////////////////////////////////////////////////////////
void LargeTerrain::onRender(Camera& camera)
{
	// Update tile maps if viewpoint changed
	if (m_viewpointChanged)
	{
		// Load and unload tile maps
		updateTileMaps(Vector2u(0), 0);

		// Must reset flag when using it
		m_viewpointChanged = false;
	}

	// Update load tasks in render thread
	updateLoadTasks();

	// Update redirect map if it has changed
	if (m_redirectMapChanged)
	{
		// Update
		m_redirectMap.update(m_redirectMapImg.getData());

		m_redirectMapChanged = false;
	}

	// Get shader
	if (!m_shader)
		m_shader = &getShader();

	// Bind shader
	m_shader->bind();

	m_shader->setUniform("u_cacheMapSize", (Vector2f)m_cacheMapSize);

	// Bind textures
	m_shader->setUniform("u_redirectMap", m_redirectMap);
	if (m_heightMap.getId())
		m_shader->setUniform("u_heightMap", m_heightMap);
	if (m_normalMap.getId())
		m_shader->setUniform("u_normalMap", m_normalMap);
}


///////////////////////////////////////////////////////////
void LargeTerrain::updateTileMaps(const Vector2u& node, Uint32 lod)
{
	// Calculate node properties
	Uint32 numNodesPerEdge = 1 << lod;
	float nodeSize = m_size / (float)numNodesPerEdge;
	float halfNodeSize = 0.5f * nodeSize;
	Vector2f center = nodeSize * (Vector2f(node.y, node.x) - (float)(numNodesPerEdge / 2) + 0.5f);
	if (lod == 0)
		center = Vector2f(0.0f);

	// Get bounding box
	BoundingBox bbox;
	{
		std::unique_lock<std::mutex> lock(m_mutex);

		const Vector2<Uint16>& hbounds = m_lodLevels[lod].m_heightBounds[node.x][node.y];

		bbox.m_min = Vector3f(center.x - halfNodeSize, (float)hbounds.x / 65535.0f * m_maxHeight, center.y - halfNodeSize);
		bbox.m_max = Vector3f(center.x + halfNodeSize, (float)hbounds.y / 65535.0f * m_maxHeight, center.y + halfNodeSize);
	}

	// Check if bounding box is inside the current lod range
	if (intersects(m_viewpoint, m_lodLevels[lod + 1].m_dist, bbox))
	{
		Vector3<Uint16> tileData(node.x, node.y, lod);

		// Check if this tile has already been loaded, if not loaded
		auto it = m_tileMap.find(tileData);
		if (it == m_tileMap.end())
		{
			// Add mapping
			Tile tile;
			tile.m_mapData.resize(3, MapData{ 0, 0, 0 });
			tile.m_cachePos = m_freeList.top();
			tile.m_tileData = tileData;
			tile.m_isLoaded = 0;
			m_tileMap[tileData] = tile;

			// Add load tasks
			addLoadTask(node, lod, &m_heightMap, m_heightLoadFunc, MapData::Height);

			// Remove from free list
			m_freeList.pop();
		}

		// Check children nodes
		if (lod < m_baseTileLevel)
		{
			Vector2u childNode = 2u * node;
			updateTileMaps(childNode + Vector2u(0, 0), lod + 1);
			updateTileMaps(childNode + Vector2u(0, 1), lod + 1);
			updateTileMaps(childNode + Vector2u(1, 0), lod + 1);
			updateTileMaps(childNode + Vector2u(1, 1), lod + 1);
		}
	}

	// Otherwise, check the lod level
	else if (lod != 0 && !intersects(m_viewpoint, m_lodLevels[lod + 1].m_dist * 1.1f, bbox))
	{
		// The base level tile map should never be freed
		Vector3<Uint16> tileData(node.x, node.y, lod);

		// Check if this tile is fully loaded
		auto it = m_tileMap.find(tileData);
		if (it != m_tileMap.end())
		{
			Tile& tile = it.value();

			// Add the cache position to free list
			m_freeList.push(tile.m_cachePos);

			// Get parent tile cache position
			Vector2<Uint8> parentCachePos = m_tileMap[Vector3<Uint16>(node.x / 2, node.y / 2, lod - 1)].m_cachePos;

			// Reset redirect map tiles
			Uint32 tileSize = 1 << (m_baseTileLevel - lod);
			Uint32 sr = node.x * tileSize;
			Uint32 sc = node.y * tileSize;
			Uint32 fr = (node.x + 1) * tileSize;
			Uint32 fc = (node.y + 1) * tileSize;

			for (Uint32 r = sr; r < fr; ++r)
			{
				for (Uint32 c = sc; c < fc; ++c)
					m_redirectMapImg[r][c] = Vector3<Uint8>(parentCachePos, m_baseTileLevel - lod + 1);
			}

			// Indicate that redirect map has changed
			m_redirectMapChanged = true;

			// Free images
			for (Uint32 i = 0; i < tile.m_mapData.size(); ++i)
			{
				MapData& mapData = tile.m_mapData[i];

				if (mapData.m_fullImg)
					Pool<Image>::free(mapData.m_fullImg);

				// Free edge image
				if (mapData.m_edgeImg)
					Pool<Image>::free(mapData.m_edgeImg);

				mapData.m_fullImg = 0;
				mapData.m_edgeImg = 0;
			}

			// Remove mapping
			m_tileMap.erase(it);

			// Call unload function
			if (m_unloadFunc)
			{
				// Calculate tile coordinates
				Uint32 numNodesPerEdge = 1 << lod;
				Vector2i tileXy = Vector2i(node.y, node.x) - (int)numNodesPerEdge / 2;

				m_unloadFunc(tileXy, lod);
			}
		}
	}
}


///////////////////////////////////////////////////////////
void LargeTerrain::updateLoadTasks()
{
	// Don't do anything if there are no load tasks
	if (!m_loadTasks.size()) return;

	// Remove list
	std::vector<Uint32> removeList;

	// Update tasks in reverse order
	for (int i = 0; i < (int)m_loadTasks.size(); ++i)
	{
		LoadTask* loadTask = m_loadTasks[i];

		// Check if the task is finished
		if (!loadTask->m_task.isFinished())
			continue;

		// The loaded image must be a square
		Image* loadedImage = loadTask->m_image;
		ASSERT(loadedImage->getWidth() == loadedImage->getHeight(), "Terrain map tiles must be sqaure");

		// Make sure texture is correct size
		ASSERT(
			!loadTask->m_texture->getId() || (
				loadTask->m_texture->getWidth() / (loadedImage->getWidth() + 2) == m_cacheMapSize.x &&
				loadTask->m_texture->getHeight() / (loadedImage->getHeight() + 2) == m_cacheMapSize.y),
			"Terrain map tile sizes must be consistent (%d px)", loadedImage->getWidth());

		// Skip and remove if the image failed to load, and also catches
		// extreme edge case: the tile went into unload range while it was still loading
		auto currentTileIt = m_tileMap.find(loadTask->m_tileData);
		if (!loadTask->m_task.getResult() || currentTileIt == m_tileMap.end())
		{
			// Remove task
			delete loadTask;
			m_loadTasks.erase(m_loadTasks.begin() + i);

			// Decrement so that next element is not skipped
			--i;

			continue;
		}
		Tile& tile = currentTileIt.value();


		// The load task is ready to be processed, create the map tile
		Uint32 mapSize = loadedImage->getWidth();

		// Calculate some metadata
		Uint32 c = loadedImage->getNumChannels();
		GLType dtype = loadedImage->getDataType();
		Uint32 typeSize = 1;
		if (dtype == GLType::Uint16 || dtype == GLType::Int16)
			typeSize = 2;
		else if (dtype == GLType::Uint32 || dtype == GLType::Int32 || dtype == GLType::Float)
			typeSize = 4;

		Uint32 pixelSize = c * typeSize;
		Uint32 rowSize = mapSize * pixelSize;

		MapData::Type mapType = loadTask->m_mapType;
		Uint32 mapTypeBitfield = 1 << mapType;
		MapData& mapData = tile.m_mapData[mapType];


		// Create edge image if it hasn't been created yet
		if (!mapData.m_edgeImg)
		{
			// Create edge image
			mapData.m_edgeImg = Pool<Image>::alloc();
			mapData.m_edgeImg->create(NULL, mapSize, 8, c, dtype);
			Uint8* edgeImgData = (Uint8*)mapData.m_edgeImg->getData();

			// Copy horizontal edges
			Uint8* src = (Uint8*)loadedImage->getData();
			memcpy(edgeImgData + (Uint32)EdgeRow::Top * rowSize, src + 0 * rowSize, rowSize);
			memcpy(edgeImgData + (Uint32)EdgeRow::Bottom * rowSize, src + (mapSize - 1) * rowSize, rowSize);
			memcpy(edgeImgData + (Uint32)EdgeRow::TMid * rowSize, src + (mapSize / 2 - 1) * rowSize, rowSize);
			memcpy(edgeImgData + (Uint32)EdgeRow::BMid * rowSize, src + (mapSize / 2) * rowSize, rowSize);

			// Copy vertical edges
			for (Uint32 x = 0; x < mapSize; ++x)
			{
				memcpy(edgeImgData + (Uint32)EdgeRow::Left * rowSize + x * pixelSize, src + x * rowSize + 0 * pixelSize, pixelSize);
				memcpy(edgeImgData + (Uint32)EdgeRow::Right * rowSize + x * pixelSize, src + x * rowSize + (mapSize - 1) * pixelSize, pixelSize);
				memcpy(edgeImgData + (Uint32)EdgeRow::LMid * rowSize + x * pixelSize, src + x * rowSize + (mapSize / 2 - 1) * pixelSize, pixelSize);
				memcpy(edgeImgData + (Uint32)EdgeRow::RMid * rowSize + x * pixelSize, src + x * rowSize + (mapSize / 2) * pixelSize, pixelSize);
			}
		}


		// Check if surrounding tiles have been loaded
		const Vector3<Uint16>& tileData = loadTask->m_tileData;
		Uint32 numTilesPerEdge = 1 << tileData.z;
		Tile* ltile = 0;
		Tile* rtile = 0;
		Tile* ttile = 0;
		Tile* btile = 0;

		if (tileData.z > 0)
		{
			// Top
			if (tileData.x > 0)
			{
				// Get tile
				ttile = getAdjTile(Vector3<Uint16>(tileData.x - 1, tileData.y, tileData.z));
				if (!ttile || !ttile->m_mapData[mapType].m_edgeImg)
					// Skip if the tile hasn't started loading, or if the edge image hasn't been created yet
					continue;
			}
			else
				ttile = &tile;

			// Bot
			if (tileData.x < numTilesPerEdge - 1)
			{
				btile = getAdjTile(Vector3<Uint16>(tileData.x + 1, tileData.y, tileData.z));
				if (!btile || !btile->m_mapData[mapType].m_edgeImg)
					continue;
			}
			else
				btile = &tile;

			// Left
			if (tileData.y > 0)
			{
				ltile = getAdjTile(Vector3<Uint16>(tileData.x, tileData.y - 1, tileData.z));
				if (!ltile || !ltile->m_mapData[mapType].m_edgeImg)
					continue;
			}
			else
				ltile = &tile;

			// Right
			if (tileData.y < numTilesPerEdge - 1)
			{
				rtile = getAdjTile(Vector3<Uint16>(tileData.x, tileData.y + 1, tileData.z));
				if (!rtile || !rtile->m_mapData[mapType].m_edgeImg)
					continue;
			}
			else
				rtile = &tile;
		}


		// Create a new temporary image with extra padding
		Image mapTile;
		mapTile.create(NULL, mapSize + 2, mapSize + 2, loadedImage->getNumChannels(), loadedImage->getDataType());

		// Copy main tile
		Uint8* src = (Uint8*)loadedImage->getData();
		Uint8* dst = (Uint8*)mapTile.getData();
		for (Uint32 r = 0; r < mapSize; ++r)
			memcpy(dst + ((r + 1) * (mapSize + 2) + 1) * pixelSize, src + (r * mapSize) * pixelSize, rowSize);

		// Set up edge pixels
		if (tileData.z == 0)
		{
			// Set up for base level (clamp to edge)
			for (Uint32 x = 0; x < mapSize; ++x)
			{
				float xf = (float)x / mapSize;
				copyPixel(loadedImage, &mapTile, Vector2f(0.0f, 1.0f - xf), Vector2u(x + 1, 0));
				copyPixel(loadedImage, &mapTile, Vector2f(1.0f, 1.0f - xf), Vector2u(x + 1, mapSize + 1));
				copyPixel(loadedImage, &mapTile, Vector2f(xf, 1.0f), Vector2u(0, x + 1));
				copyPixel(loadedImage, &mapTile, Vector2f(xf, 0.0f), Vector2u(mapSize + 1, x + 1));
			}

			// For corners, set value to the mean of the two adjacent tiles (to keep this part simple)
			fillCorner(&mapTile, Vector2u(0, 0), Vector2u(1, 0), Vector2u(0, 1));
			fillCorner(&mapTile, Vector2u(mapSize + 1, 0), Vector2u(mapSize, 0), Vector2u(mapSize + 1, 1));
			fillCorner(&mapTile, Vector2u(0, mapSize + 1), Vector2u(1, mapSize + 1), Vector2u(0, mapSize));
			fillCorner(&mapTile, Vector2u(mapSize + 1, mapSize + 1), Vector2u(mapSize, mapSize + 1), Vector2u(mapSize + 1, mapSize));
		}
		else
		{
			// Set up for other levels

			// Determine which edge to sample from for each edge and the offsets/scales
			const Vector3<Uint16>& ldata = ltile->m_tileData;
			const Vector3<Uint16>& rdata = rtile->m_tileData;
			const Vector3<Uint16>& tdata = ttile->m_tileData;
			const Vector3<Uint16>& bdata = btile->m_tileData;

			bool lIsBigger = ldata.z == tileData.z - 1;
			bool rIsBigger = rdata.z == tileData.z - 1;
			bool tIsBigger = tdata.z == tileData.z - 1;
			bool bIsBigger = bdata.z == tileData.z - 1;

			float loffset = lIsBigger && tileData.x % 2 != 0 ? 0.5f : 0.0f;
			float roffset = rIsBigger && tileData.x % 2 != 0 ? 0.5f : 0.0f;
			float toffset = tIsBigger && tileData.y % 2 != 0 ? 0.5f : 0.0f;
			float boffset = bIsBigger && tileData.y % 2 != 0 ? 0.5f : 0.0f;

			float lscale = lIsBigger ? 0.5f : 1.0f;
			float rscale = rIsBigger ? 0.5f : 1.0f;
			float tscale = tIsBigger ? 0.5f : 1.0f;
			float bscale = bIsBigger ? 0.5f : 1.0f;

			float lrow = (float)(lIsBigger && tileData.y % 2 != 0 ? EdgeRow::LMid : EdgeRow::Right) / 8.0f + 0.5f / 8.0f;
			float rrow = (float)(!rIsBigger || tileData.y % 2 != 0 ? EdgeRow::Left : EdgeRow::RMid) / 8.0f + 0.5f / 8.0f;
			float trow = (float)(tIsBigger && tileData.x % 2 != 0 ? EdgeRow::TMid : EdgeRow::Bottom) / 8.0f + 0.5f / 8.0f;
			float brow = (float)(!bIsBigger || tileData.x % 2 != 0 ? EdgeRow::Top : EdgeRow::BMid) / 8.0f + 0.5f / 8.0f;

			// Correction for when the tile is along the edge of the terrain
			if (ltile == &tile) lrow = (float)EdgeRow::Left / 8.0f + 0.5f / 8.0f;
			if (rtile == &tile) rrow = (float)EdgeRow::Right / 8.0f + 0.5f / 8.0f;
			if (ttile == &tile) trow = (float)EdgeRow::Top / 8.0f + 0.5f / 8.0f;
			if (btile == &tile) brow = (float)EdgeRow::Bottom / 8.0f + 0.5f / 8.0f;

			MapData& lMapData = ltile->m_mapData[mapType];
			MapData& rMapData = rtile->m_mapData[mapType];
			MapData& tMapData = ttile->m_mapData[mapType];
			MapData& bMapData = btile->m_mapData[mapType];

			Image* limg = lMapData.m_edgeImg;
			Image* rimg = rMapData.m_edgeImg;
			Image* timg = tMapData.m_edgeImg;
			Image* bimg = bMapData.m_edgeImg;

			// Copy pixel by pixel
			for (Uint32 x = 0; x < mapSize; ++x)
			{
				float xf = (float)x + 0.5f;
				copyPixel(limg, &mapTile, Vector2f(loffset + lscale * xf / mapSize, 1.0f - lrow), Vector2u(x + 1, 0));
				copyPixel(rimg, &mapTile, Vector2f(roffset + rscale * xf / mapSize, 1.0f - rrow), Vector2u(x + 1, mapSize + 1));
				copyPixel(timg, &mapTile, Vector2f(toffset + tscale * xf / mapSize, 1.0f - trow), Vector2u(0, x + 1));
				copyPixel(bimg, &mapTile, Vector2f(boffset + bscale * xf / mapSize, 1.0f - brow), Vector2u(mapSize + 1, x + 1));
			}

			// For corners, set value to the mean of the two adjacent tiles (to keep this part simple)
			fillCorner(&mapTile, Vector2u(0, 0), Vector2u(1, 0), Vector2u(0, 1));
			fillCorner(&mapTile, Vector2u(mapSize + 1, 0), Vector2u(mapSize, 0), Vector2u(mapSize + 1, 1));
			fillCorner(&mapTile, Vector2u(0, mapSize + 1), Vector2u(1, mapSize + 1), Vector2u(0, mapSize));
			fillCorner(&mapTile, Vector2u(mapSize + 1, mapSize + 1), Vector2u(mapSize, mapSize + 1), Vector2u(mapSize + 1, mapSize));

			// Update adjacent tiles if a higher edge resolution can be used
			if (!lIsBigger && (ltile->m_isLoaded & mapTypeBitfield) && lMapData.m_edgeResR < tileData.z)
			{
				Vector2u cachePosXy = Vector2u(ltile->m_cachePos) * (mapSize + 2) + Vector2u(mapSize + 1, 1);
				Uint8* srcRow = (Uint8*)mapData.m_edgeImg->getData() + (int)EdgeRow::Left * rowSize;
				lMapData.m_texture->update(srcRow, cachePosXy, Vector2u(1, mapSize));

				lMapData.m_edgeResR = (Uint8)tileData.z;
			}

			if (!rIsBigger && (rtile->m_isLoaded & mapTypeBitfield) && rMapData.m_edgeResL < tileData.z)
			{
				Vector2u cachePosXy = Vector2u(rtile->m_cachePos) * (mapSize + 2) + Vector2u(0, 1);
				Uint8* srcRow = (Uint8*)mapData.m_edgeImg->getData() + (int)EdgeRow::Right * rowSize;
				rMapData.m_texture->update(srcRow, cachePosXy, Vector2u(1, mapSize));

				rMapData.m_edgeResL = (Uint8)tileData.z;
			}

			if (!tIsBigger && (ttile->m_isLoaded & mapTypeBitfield) && tMapData.m_edgeResB < tileData.z)
			{
				Vector2u cachePosXy = Vector2u(ttile->m_cachePos) * (mapSize + 2) + Vector2u(1, mapSize + 1);
				Uint8* srcRow = (Uint8*)mapData.m_edgeImg->getData() + (int)EdgeRow::Top * rowSize;
				tMapData.m_texture->update(srcRow, cachePosXy, Vector2u(mapSize, 1));

				tMapData.m_edgeResB = (Uint8)tileData.z;
			}

			if (!bIsBigger && (btile->m_isLoaded & mapTypeBitfield) && bMapData.m_edgeResT < tileData.z)
			{
				Vector2u cachePosXy = Vector2u(btile->m_cachePos) * (mapSize + 2) + Vector2u(1, 0);
				Uint8* srcRow = (Uint8*)mapData.m_edgeImg->getData() + (int)EdgeRow::Bottom * rowSize;
				bMapData.m_texture->update(srcRow, cachePosXy, Vector2u(mapSize, 1));

				bMapData.m_edgeResT = (Uint8)tileData.z;
			}

			// Set edge resolutions
			mapData.m_edgeResL = (Uint8)ldata.z;
			mapData.m_edgeResR = (Uint8)rdata.z;
			mapData.m_edgeResT = (Uint8)tdata.z;
			mapData.m_edgeResB = (Uint8)bdata.z;
		}

		// Create texture if it hasn't yet (this is the first time it can be created without wasting loading resources)
		// because this is the first time the tile size for the texture will be known
		if (!loadTask->m_texture->getId())
		{
			// Get texture pixel format
			PixelFormat fmt = PixelFormat::R;
			if (c == 2)
				fmt = PixelFormat::Rg;
			else if (c == 3)
				fmt = PixelFormat::Rgb;
			else if (c == 4)
				fmt = PixelFormat::Rgba;

			// Create texture
			loadTask->m_texture->create(NULL, fmt, (mapSize + 2) * m_cacheMapSize.x, (mapSize + 2) * m_cacheMapSize.y, 0, dtype);
		}

		// Upload to cache texture (cache position is already in xy coordinates)
		Vector2u cachePosXy = Vector2u(tile.m_cachePos) * (mapSize + 2);
		loadTask->m_texture->update(mapTile.getData(), cachePosXy, Vector2u(mapSize + 2));

		// Set map data's texture
		mapData.m_texture = loadTask->m_texture;


		// If the current tile is uploading to the height map, keep the loaded image to generate normal map
		if (loadTask->m_texture == &m_heightMap)
		{
			// Move the map tile image into the loaded image spot
			*loadedImage = std::move(mapTile);

			// Create new image for normals tile
			Image* normalImg = Pool<Image>::alloc();

			// Create normals tile task
			LoadTask* normalsTask = new LoadTask();
			normalsTask->m_image = normalImg;
			normalsTask->m_texture = &m_normalMap;
			normalsTask->m_tileData = tileData;
			normalsTask->m_mapType = MapData::Normal;

			// Call the generate normals task
			normalsTask->m_task = Scheduler::addTask(&LargeTerrain::processHeightTile, this, loadedImage, normalImg, tileData);

			// Add to task list
			m_loadTasks.push_back(normalsTask);
		}
		else
		{
			// Otherwise, free the loaded image
			Pool<Image>::free(loadedImage);
		}

		// Mark this tile as loaded
		tile.m_isLoaded |= mapTypeBitfield;

		// If all tile maps for the current tile has been loaded, update the redirect map
		if (tile.m_isLoaded == m_tileLoadedBitfield)
		{
			Uint32 tileSize = 1 << (m_baseTileLevel - tileData.z);
			Uint32 sr = tileData.x * tileSize;
			Uint32 sc = tileData.y * tileSize;
			Uint32 fr = (tileData.x + 1) * tileSize;
			Uint32 fc = (tileData.y + 1) * tileSize;

			for (Uint32 r = sr; r < fr; ++r)
			{
				for (Uint32 c = sc; c < fc; ++c)
					m_redirectMapImg[r][c] = Vector3<Uint8>(tile.m_cachePos, m_baseTileLevel - tileData.z);
			}

			// Indicate that redirect map has changed
			m_redirectMapChanged = true;
		}

		// Remove load task
		delete loadTask;
		m_loadTasks.erase(m_loadTasks.begin() + i);

		// Decrement so that next element is not skipped
		--i;
	}
}


///////////////////////////////////////////////////////////
void LargeTerrain::addLoadTask(const Vector2u& node, Uint32 lod, Texture* texture, const LoadFunc& func, Uint32 mapType)
{
	Uint32 numNodesPerEdge = 1 << lod;
	Vector2i tileXy = Vector2i(node.y, node.x) - (int)numNodesPerEdge / 2;

	// Create task
	LoadTask* task = new LoadTask();
	task->m_image = Pool<Image>::alloc();
	task->m_texture = texture;
	task->m_tileData = Vector3<Uint16>(node.x, node.y, lod);
	task->m_mapType = (MapData::Type)mapType;

	// Call load function
	task->m_task = Scheduler::addTask(m_heightLoadFunc, tileXy, lod, task->m_image);

	// Add to list
	m_loadTasks.push_back(task);
}


///////////////////////////////////////////////////////////
LargeTerrain::Tile* LargeTerrain::getAdjTile(const Vector3<Uint16>& tileData)
{
	Tile* tile = 0;

	auto it = m_tileMap.find(tileData);
	if (it != m_tileMap.end())
		tile = &it.value();
	else
	{
		it = m_tileMap.find(Vector3<Uint16>(tileData.x / 2, tileData.y / 2, tileData.z - 1));
		if (it != m_tileMap.end())
			tile = &it.value();
	}

	return tile;
}


///////////////////////////////////////////////////////////
bool LargeTerrain::processHeightTile(Image* hmap, Image* nmap, const Vector3<Uint16>& tile)
{
	// Get buffer for height map
	ImageBuffer<float> heights = hmap->getBuffer<float>();

	// Create normals tile
	Uint32 mapSize = hmap->getWidth() - 2;
	nmap->create(NULL, mapSize, mapSize, 3, GLType::Uint16);
	ImageBuffer<Vector3<Uint16>> normals = nmap->getBuffer<Vector3<Uint16>>();

	// Calculate size factor
	Uint32 numNodesPerEdge = 1 << tile.z;
	float sizeFactor = m_size / (mapSize * numNodesPerEdge);

	for (Uint32 r = 0; r < mapSize; ++r)
	{
		for (Uint32 c = 0; c < mapSize; ++c)
		{
			// Calculate normal
			float h01 = heights[r + 1][c] * m_maxHeight;
			float h21 = heights[r + 1][c + 2] * m_maxHeight;
			float h10 = heights[r + 2][c + 1] * m_maxHeight;
			float h12 = heights[r][c + 1] * m_maxHeight;

			Vector3f v1(sizeFactor, h21 - h01, 0.0f);
			Vector3f v2(0.0f, h12 - h10, -sizeFactor);
			Vector3f normal = normalize(cross(v1, v2));

			// Adjust for 16-bit
			normal.x = 0.5f * normal.x + 0.5f;
			normal.z = 0.5f * normal.z + 0.5f;

			normals[r][c] = Vector3<Uint16>(normal * 65535.0f);
		}
	}

	// If processing a base level height tile, then do a few extra procedures
	if (tile.z == m_baseTileLevel)
	{
		// Number of base terrain nodes per base map tile edge
		Uint32 numNodesPerEdge = 1 << (m_lodLevels.size() - m_baseTileLevel - 1);

		// Create a local bounds map to minimize time spent using shared resources
		ImageBuffer<Vector2<Uint16>> localBounds(numNodesPerEdge, Vector2<Uint16>(65535, 0));

		for (Uint32 r = 0; r < numNodesPerEdge; ++r)
		{
			for (Uint32 c = 0; c < numNodesPerEdge; ++c)
			{
				// Map map tile coordinates to image coordinates
				Uint32 rs = (Uint32)std::floor((float)r / numNodesPerEdge * mapSize) + 1;
				Uint32 cs = (Uint32)std::floor((float)c / numNodesPerEdge * mapSize) + 1;
				Uint32 rf = (Uint32)std::ceil((float)(r + 1) / numNodesPerEdge * mapSize) + 1;
				Uint32 cf = (Uint32)std::ceil((float)(c + 1) / numNodesPerEdge * mapSize) + 1;

				Vector2f bounds(1.0f, 0.0f);

				for (Uint32 ri = rs; ri < rf; ++ri)
				{
					for (Uint32 ci = cs; ci < cf; ++ci)
					{
						// Get height pixel
						float h = *(float*)hmap->getPixel(ri, ci);

						// Record bounds
						if (h < bounds.x)
							bounds.x = h;
						if (h > bounds.y)
							bounds.y = h;
					}
				}

				// Update bounds value
				localBounds[r][c] = Vector2<Uint16>(bounds * 65535.0f);
			}
		}

		// Copy local bounds map to actual map
		std::unique_lock<std::mutex> lock(m_mutex);

		// Calculate starting position of tiles
		ImageBuffer<Vector2<Uint16>>& baseBounds = m_lodLevels.back().m_heightBounds;
		Uint32 rs = tile.x * numNodesPerEdge;
		Uint32 cs = tile.y * numNodesPerEdge;

		// Update base bounds
		for (Uint32 r = 0; r < numNodesPerEdge; ++r)
		{
			for (Uint32 c = 0; c < numNodesPerEdge; ++c)
			{
				Vector2<Uint16>& base = baseBounds[rs + r][cs + c];
				Vector2<Uint16>& local = localBounds[r][c];

				if (local.x < base.x)
					base.x = local.x;
				if (local.y > base.y)
					base.y = local.y;
			}
		}

		// Propogate up to base map level
		for (int i = m_lodLevels.size() - 2; i >= (int)m_baseTileLevel; --i)
		{
			ImageBuffer<Vector2<Uint16>>& prevBounds = m_lodLevels[i + 1].m_heightBounds;
			ImageBuffer<Vector2<Uint16>>& currBounds = m_lodLevels[i].m_heightBounds;

			// Update parameters
			numNodesPerEdge /= 2;
			rs /= 2;
			cs /= 2;

			for (Uint32 r = rs; r < rs + numNodesPerEdge; ++r)
			{
				for (Uint32 c = cs; c < cs + numNodesPerEdge; ++c)
				{
					Vector2<Uint16>& b1 = prevBounds[2 * r + 0][2 * c + 0];
					Vector2<Uint16>& b2 = prevBounds[2 * r + 0][2 * c + 1];
					Vector2<Uint16>& b3 = prevBounds[2 * r + 1][2 * c + 0];
					Vector2<Uint16>& b4 = prevBounds[2 * r + 1][2 * c + 1];

					// Apply mins and maxes
					Vector2<Uint16>& bounds = currBounds[r][c] = Vector2<Uint16>(65535, 0);

					if (b1.x < bounds.x)
						bounds.x = b1.x;
					if (b2.x < bounds.x)
						bounds.x = b2.x;
					if (b3.x < bounds.x)
						bounds.x = b3.x;
					if (b4.x < bounds.x)
						bounds.x = b4.x;

					if (b1.y > bounds.y)
						bounds.y = b1.y;
					if (b2.y > bounds.y)
						bounds.y = b2.y;
					if (b3.y > bounds.y)
						bounds.y = b3.y;
					if (b4.y > bounds.y)
						bounds.y = b4.y;
				}
			}
		}

		// Propogate up to root level
		for (int i = m_baseTileLevel - 1; i >= 0; --i)
		{
			Vector2<Uint16>& prevBounds = m_lodLevels[i + 1].m_heightBounds[rs][cs];
			Vector2<Uint16>& currBounds = m_lodLevels[i].m_heightBounds[rs / 2][cs / 2];

			// Update parameters
			rs /= 2;
			cs /= 2;

			// Apply min and max
			if (prevBounds.x < currBounds.x)
				currBounds.x = prevBounds.x;
			if (prevBounds.y > currBounds.y)
				currBounds.y = prevBounds.y;
		}
	}

	// TODO : Don't free height map to use for terrain colliders
	Pool<Image>::free(hmap);

	return true;
}


///////////////////////////////////////////////////////////
void LargeTerrain::setHeightLoader(const LoadFunc& func)
{
	m_heightLoadFunc = func;
	m_tileLoadedBitfield |= MapData::Height | MapData::Normal;

	// Load base tile to fill height bounds map with bounds
	Image hmap;
	if (!m_heightLoadFunc(Vector2i(0), 0, &hmap))
		return;

	// Make sure tile is correct
	ASSERT(hmap.getWidth() == hmap.getHeight(), "Terrain map tiles must be sqaure");
	ASSERT(hmap.getDataType() == GLType::Float, "Large terrain height maps must use the float data type");
	ASSERT(hmap.getNumChannels() == 1, "Large terrain height maps must use a single color channel");

	// Determine the number of steps each base level tile needs
	float baseLevelSize = m_size / (float)(1 << (m_lodLevels.size() - 1));
	float numSteps = baseLevelSize * (float)hmap.getWidth() / m_size;

	// Fill in base bounds
	std::unique_lock<std::mutex> lock(m_mutex);

	ImageBuffer<Vector2<Uint16>>& baseBounds = m_lodLevels.back().m_heightBounds;

	for (Uint32 r = 0; r < baseBounds.getHeight(); ++r)
	{
		for (Uint32 c = 0; c < baseBounds.getWidth(); ++c)
		{
			float rs = (float)r * numSteps;
			float cs = (float)c * numSteps;
			float rf = (float)(r + 1) * numSteps;
			float cf = (float)(c + 1) * numSteps;

			Vector2f bounds(1.0, 0.0f);

			for (float ri = rs; ri <= rf;)
			{
				for (float ci = cs; ci <= cf;)
				{
					// Get height
					Vector2f uv = Vector2f(ci, ri) / (float)(hmap.getWidth() - 1);
					uv.y = 1.0f - uv.y;
					float h = hmap.sample<float>(uv);

					// Record max and min
					if (h < bounds.x)
						bounds.x = h;
					if (h > bounds.y)
						bounds.y = h;

					// Update c
					if (ci == cf)
						break;
					else
						ci = std::min(ci + 1.0f, cf);
				}

				// Update r
				if (ri == rf)
					break;
				else
					ri = std::min(ri + 1.0f, rf);
			}

			// Write bounds to map
			baseBounds[r][c] = Vector2<Uint16>(bounds * 65535.0f);
		}
	}

	// Propogate height bounds up the lod levels
	for (int i = m_lodLevels.size() - 2; i >= 0; --i)
	{
		ImageBuffer<Vector2<Uint16>>& prevBounds = m_lodLevels[i + 1].m_heightBounds;
		ImageBuffer<Vector2<Uint16>>& currBounds = m_lodLevels[i].m_heightBounds;

		for (Uint32 r = 0; r < currBounds.getHeight(); ++r)
		{
			for (Uint32 c = 0; c < currBounds.getWidth(); ++c)
			{
				Vector2<Uint16>& b1 = prevBounds[2 * r + 0][2 * c + 0];
				Vector2<Uint16>& b2 = prevBounds[2 * r + 0][2 * c + 1];
				Vector2<Uint16>& b3 = prevBounds[2 * r + 1][2 * c + 0];
				Vector2<Uint16>& b4 = prevBounds[2 * r + 1][2 * c + 1];

				// Apply mins and maxes
				Vector2<Uint16>& bounds = currBounds[r][c] = Vector2<Uint16>(65535, 0);

				if (b1.x < bounds.x)
					bounds.x = b1.x;
				if (b2.x < bounds.x)
					bounds.x = b2.x;
				if (b3.x < bounds.x)
					bounds.x = b3.x;
				if (b4.x < bounds.x)
					bounds.x = b4.x;

				if (b1.y > bounds.y)
					bounds.y = b1.y;
				if (b2.y > bounds.y)
					bounds.y = b2.y;
				if (b3.y > bounds.y)
					bounds.y = b3.y;
				if (b4.y > bounds.y)
					bounds.y = b4.y;
			}
		}
	}
}


///////////////////////////////////////////////////////////
Texture& LargeTerrain::getRedirectMap()
{
	return m_redirectMap;
}


///////////////////////////////////////////////////////////
Texture& LargeTerrain::getHeightMap()
{
	return m_heightMap;
}


///////////////////////////////////////////////////////////
Texture& LargeTerrain::getNormalMap()
{
	return m_normalMap;
}


///////////////////////////////////////////////////////////
void LargeTerrain::onUnloadTile(const std::function<void(const Vector2i&, Uint32)>& func)
{
	m_unloadFunc = func;
}


}