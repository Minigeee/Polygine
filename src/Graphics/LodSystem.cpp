#include <poly/Graphics/LodSystem.h>

namespace poly
{


///////////////////////////////////////////////////////////
void LodSystem::addLevel(float dist, Renderable* renderable, Shader* shader)
{
	// Add an lod level
	m_lodLevels.push_back(LodLevel{ renderable, dist });

	// Sort levels by distance
	std::sort(m_lodLevels.begin(), m_lodLevels.end(),
		[](const LodLevel& a, const LodLevel& b) -> bool
		{
			return a.m_distance < b.m_distance;
		}
	);
}


///////////////////////////////////////////////////////////
Uint32 LodSystem::getNumLevels() const
{
	return m_lodLevels.size();
}


///////////////////////////////////////////////////////////
float LodSystem::getDistance(Uint32 level) const
{
	return m_lodLevels[level].m_distance;
}


///////////////////////////////////////////////////////////
Renderable* LodSystem::getRenderable(float dist) const
{
	// Find the correct lod level
	Uint32 i = 0;
	for (; i < m_lodLevels.size() && dist > m_lodLevels[i].m_distance; ++i);

	// Return null if the distance is too far
	return i < m_lodLevels.size() ? m_lodLevels[i].m_renderable : 0;
}


///////////////////////////////////////////////////////////
Renderable* LodSystem::getRenderable(Uint32 level) const
{
	return m_lodLevels[level].m_renderable;
}



///////////////////////////////////////////////////////////
const BoundingBox& LodSystem::getBoundingBox() const
{
	return m_lodLevels.size() ? m_lodLevels[0].m_renderable->getBoundingBox() : m_boundingBox;
}


///////////////////////////////////////////////////////////
const Sphere& LodSystem::getBoundingSphere() const
{
	return m_lodLevels.size() ? m_lodLevels[0].m_renderable->getBoundingSphere() : m_boundingSphere;
}


}