#include <poly/Core/Profiler.h>

#include <poly/Engine/Components.h>
#include <poly/Engine/Scene.h>

namespace poly
{


///////////////////////////////////////////////////////////
HandleArray<bool> Scene::idArray;


///////////////////////////////////////////////////////////
Scene::Scene() :
	m_handle				(idArray.add(true))
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
	it.value().removeEntity(id);
}


///////////////////////////////////////////////////////////
void Scene::removeQueuedEntities()
{
	// Entity removal is protected by mutex
	std::lock_guard<std::mutex> lock(m_entityMutex);

	// Clear all queues
	for (auto it = m_entityGroups.begin(); it != m_entityGroups.end(); ++it)
		it.value().removeQueuedEntities();
}


///////////////////////////////////////////////////////////
void Scene::addTag(Entity::Id id, int tag)
{
	std::lock_guard<std::mutex> lock(m_tagMutex);

	// Insert id into correct tag group
	m_entityTags[tag].insert(id);
}


///////////////////////////////////////////////////////////
void Scene::removeTag(Entity::Id id, int tag)
{
	std::lock_guard<std::mutex> lock(m_tagMutex);

	// Remove entity from tag group
	m_entityTags[tag].erase(id);
}


///////////////////////////////////////////////////////////
bool Scene::hasTag(Entity::Id id, int tag)
{
	std::lock_guard<std::mutex> lock(m_tagMutex);

	// Get the tag group
	auto it = m_entityTags.find(tag);
	if (it == m_entityTags.end()) return false;

	// Return if the id has been found
	return it->second.find(id) != it->second.end();
}


///////////////////////////////////////////////////////////
const HashSet<Entity::Id>& Scene::getEntitiesWithTag(Uint32 tag)
{
	return m_entityTags[tag];
}

}