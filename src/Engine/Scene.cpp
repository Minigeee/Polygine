#include <poly/Engine/Scene.h>

namespace poly
{

HandleArray<bool> Scene::idArray;

///////////////////////////////////////////////////////////

Scene::Scene() :
	m_handle	(idArray.add(true))
{ }

Scene::~Scene()
{
	// Remove all entities
	for (auto it = m_entityGroups.begin(); it != m_entityGroups.end(); ++it)
	{
		priv::EntityGroup& group = it->second;

		// Get list of entity ids
		const std::vector<Entity::Id>& ids = group.getEntityIds();
		// Add all to the remove queue
		for (Uint32 i = 0; i < ids.size(); ++i)
			group.removeEntity(ids[i]);

		// Remove queued entities
		group.removeQueuedEntities();
	}

	// Remove from list to free up id
	idArray.remove(m_handle);
}

Uint16 Scene::getId() const
{
	return m_handle.m_index;
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