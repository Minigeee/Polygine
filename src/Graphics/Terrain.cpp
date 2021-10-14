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

namespace poly
{


///////////////////////////////////////////////////////////
Shader Terrain::s_shader;


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
Terrain::Terrain() :
	m_shader				(0),
	m_size					(0.0f),
	m_maxHeight				(0.0f),
	m_viewpoint				(0.0f),
	m_instanceDataOffset	(0),
	m_useCustomViewpoint	(false),
	m_baseScale				(1.0f)
{

}


///////////////////////////////////////////////////////////
Terrain::~Terrain()
{

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
		m_lodLevels.back().m_heightBounds.resize(numNodes, Vector2<Uint16>(65535, 0));

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
		m_shader = &getDefaultShader();

		// Set lod ranges a single time
		m_shader->bind();
		for (Uint32 i = 0; i < m_lodLevels.size(); ++i)
			m_shader->setUniform("u_lodRanges[" + std::to_string(i) + ']', m_lodLevels[i].m_range);
		m_shader->setUniform("u_lodRanges[" + std::to_string(m_lodLevels.size()) + ']', 0.0f);
	}

	START_PROFILING_FUNC;


	// Only update viewpoint for default render passes
	if (pass == RenderPass::Default && !m_useCustomViewpoint)
		m_viewpoint = camera.getPosition();

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
	m_shader->setUniform("u_heightMap", m_heightMap);
	m_shader->setUniform("u_normalMap", m_normalMap);

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
void Terrain::setViewpoint(const Vector3f& point)
{
	m_viewpoint = point;
	m_useCustomViewpoint = true;
}


///////////////////////////////////////////////////////////
void Terrain::setHeightMap(const Image& hmap)
{
	// Make sure image has correct requirements
	ASSERT(hmap.getWidth() == hmap.getHeight(), "Height maps must be square");
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
	m_heightMap.create(m_heightMapImg);

	// Update entire map
	updateData(Vector2u(0), Vector2u(hmap.getWidth(), hmap.getHeight()));
}


///////////////////////////////////////////////////////////
void Terrain::updateData(const Vector2u& pos, const Vector2u& size)
{
	// Calculate rectangle in node coordinates
	Uint32 numTilesPerEdge = 1 << (m_lodLevels.size() - 1);
	Uint32 pixelsPerTile = m_heightMap.getWidth() / numTilesPerEdge;
	Uint32 sr = pos.y / pixelsPerTile;
	Uint32 sc = pos.x / pixelsPerTile;
	Uint32 fr = (pos.y + size.y - 1) / pixelsPerTile;
	Uint32 fc = (pos.x + size.x - 1) / pixelsPerTile;

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
	if (!m_normalMap.getId())
	{
		// Create empty image
		m_normalMapImg.create(0, mapSize.x, mapSize.y, 3, GLType::Float);

		// Create texture
		m_normalMap.create(m_normalMapImg);
	}

	// Calculate normals
	Vector3f* normals = (Vector3f*)m_normalMapImg.getData();
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

			normals[r * mapSize.x + c] = normal;
		}
	}

	// Update texture
	m_normalMap.update(normals);
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


}