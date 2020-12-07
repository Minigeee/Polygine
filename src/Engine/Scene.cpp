#include <poly/Core/Profiler.h>

#include <poly/Engine/Components.h>
#include <poly/Engine/Scene.h>

#include <poly/Graphics/RenderSystem.h>

namespace poly
{


///////////////////////////////////////////////////////////
HandleArray<bool> Scene::idArray;


///////////////////////////////////////////////////////////
E_EntitiesCreated::E_EntitiesCreated() :
	m_numEntities	(0),
	m_entities		(0)
{

}


///////////////////////////////////////////////////////////
E_EntitiesCreated::E_EntitiesCreated(std::vector<Entity>& entities) :
	m_numEntities	(entities.size()),
	m_entities		(&entities[0])
{

}


///////////////////////////////////////////////////////////
Scene::Scene() :
	m_handle				(idArray.add(true))
{

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
void Scene::addRenderSystem(RenderSystem* system)
{
	// Initialize the system
	system->init(this);

	m_renderSystems.push_back(system);
}


///////////////////////////////////////////////////////////
void Scene::render(Camera& camera, FrameBuffer& target)
{
	// Bind framebuffer
	target.bind();

	// Render state
	RenderState::Default.apply();

	// Render all render systems
	for (Uint32 i = 0; i < m_renderSystems.size(); ++i)
		m_renderSystems[i]->render(camera);
}

}