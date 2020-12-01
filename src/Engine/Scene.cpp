#include <poly/Core/Profiler.h>

#include <poly/Engine/Components.h>
#include <poly/Engine/Scene.h>

namespace poly
{


///////////////////////////////////////////////////////////
HandleArray<bool> Scene::idArray;


///////////////////////////////////////////////////////////
Scene::Scene() :
	m_handle				(idArray.add(true)),
	m_groupPool				(sizeof(priv::EntityGroup), 16)
{
	// Create stream type instance buffer
	// m_instanceBuffer.create<Matrix4f>(0, 65536, BufferUsage::Stream);
}


///////////////////////////////////////////////////////////
Scene::~Scene()
{
	// Clean up ECS
	{
		std::lock_guard<std::mutex> lock(m_entityMutex);
		priv::ComponentCleanup::cleanup(m_handle.m_index);
	}

	// Clean up event systems
	priv::SceneEventsCleanup::cleanup(m_handle.m_index);

	// Remove from list to free up id
	idArray.remove(m_handle);
}


///////////////////////////////////////////////////////////
Uint16 Scene::getId() const
{
	return m_handle.m_index;
}


///////////////////////////////////////////////////////////
void Scene::removeEntity(Entity::Id id)
{
	// Find the correct group
	Uint32 groupId = id.m_group;

	// Entity removal is protected by mutex
	std::lock_guard<std::mutex> lock(m_entityMutex);

	auto it = m_entityGroups.find(groupId);
	ASSERT(it != m_entityGroups.end(), "Can not remove entity from unknown entity group: %d", groupId);

	// This function adds it to a remove queue
	it.value()->removeEntity(id);
}


///////////////////////////////////////////////////////////
void Scene::removeQueuedEntities()
{
	// Entity removal is protected by mutex
	std::lock_guard<std::mutex> lock(m_entityMutex);

	// Clear all queues
	for (auto it = m_entityGroups.begin(); it != m_entityGroups.end(); ++it)
		it.value()->removeQueuedEntities();
}


///////////////////////////////////////////////////////////
Entity Scene::addTag(Entity::Id id, const std::string& tag)
{
	// Tag hash
	Uint32 hash = (Uint32)std::hash<std::string>()(tag);

	std::lock_guard<std::mutex> lock(m_entityMutex);

	// Find original group to check if it has a tag
	auto it = m_entityGroups.find(id.m_group);
	ASSERT(it != m_entityGroups.end(), "Entity group does not exist");
	if (it.value()->hasTag(hash))
		// Return the original id if it already has the tag
		return Entity(this, id);

	// Try to find the new group
	Uint32 groupId = id.m_group * hash;
	priv::EntityGroup* group = 0;

	it = m_entityGroups.find(groupId);
	if (it == m_entityGroups.end())
	{
		// Initialize new group
		group = m_entityGroups[groupId] = (priv::EntityGroup*)m_groupPool.alloc();
		new(group)priv::EntityGroup(this, m_handle.m_index);
		m_entityGroups[id.m_group]->copyTypeSetTo(*group, groupId);

		// Add the tag to the set
		group->addTag(hash);
	}
	else
		group = it.value();

	// Move entity
	return m_entityGroups[id.m_group]->moveEntity(id, *group);
}


///////////////////////////////////////////////////////////
bool Scene::hasTag(Entity::Id id, const std::string& tag)
{
	// Get entity group
	auto it = m_entityGroups.find(id.m_group);
	if (it == m_entityGroups.end()) return false;

	return it.value()->hasTag(std::hash<std::string>()(tag));
}

}