#include <poly/Engine/Scene.h>

namespace poly
{

///////////////////////////////////////////////////////////

Scene::Scene() :
	m_id	(0)
{ }

Uint16 Scene::getId() const
{
	return m_id;
}

///////////////////////////////////////////////////////////

void Scene::removeEntity(Entity::Id id)
{
	// Find the correct group
	Uint32 groupId = id.m_group;

	auto it = m_entityGroups.find(groupId);
	ASSERT(it != m_entityGroups.end(), "Can not remove entity from unknown entity group: %d", groupId);

	// This function adds it to a remove queue
	it->second.removeEntity(id);
}

void Scene::removeQueuedEntities()
{
	// Clear all queues
	for (auto it = m_entityGroups.begin(); it != m_entityGroups.end(); ++it)
		it->second.removeQueuedEntities();
}

///////////////////////////////////////////////////////////

}