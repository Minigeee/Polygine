#include <poly/Engine/Ecs.h>

namespace poly
{

namespace priv
{

EntityGroup::EntityGroup() :
	m_sceneId	(0),
	m_groupId	(0)
{ }

EntityGroup::EntityGroup(Uint16 sceneId) :
	m_sceneId	(sceneId),
	m_groupId	(0)
{ }

void EntityGroup::removeEntities(const std::vector<Entity::Id>& ids)
{
	// Call the remove function
	if (m_removeFunc)
		m_removeFunc(ids);
}

}

}