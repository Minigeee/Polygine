#include <poly/Engine/Ecs.h>

namespace poly
{

namespace priv
{

EntityGroup::EntityGroup() :
	m_scene		(0),
	m_sceneId	(0),
	m_groupId	(0)
{ }

EntityGroup::EntityGroup(Scene* scene, Uint16 sceneId) :
	m_scene		(scene),
	m_sceneId	(sceneId),
	m_groupId	(0)
{ }

void EntityGroup::removeEntity(Entity::Id id)
{
	m_removeQueue.push_back(id);
}

void EntityGroup::removeQueuedEntities()
{
	// Call the remove function
	if (m_removeQueue.size())
	{
		m_removeFunc(m_removeQueue);
		m_removeQueue.clear();
	}
}

}

}