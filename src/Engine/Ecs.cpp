#include <poly/Engine/Ecs.h>

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
HashMap<Uint32, std::function<void(Uint16)>> ComponentCleanup::m_cleanupFuncs;


///////////////////////////////////////////////////////////
void ComponentCleanup::cleanup(Uint16 sceneId)
{
	// Iterate hash map and call all cleanup functions
	for (auto it = m_cleanupFuncs.begin(); it != m_cleanupFuncs.end(); ++it)
		it.value()(sceneId);
}


///////////////////////////////////////////////////////////
EntityGroup::EntityGroup() :
	m_scene		(0),
	m_sceneId	(0),
	m_groupId	(0)
{ }


///////////////////////////////////////////////////////////
EntityGroup::EntityGroup(Scene* scene, Uint16 sceneId) :
	m_scene		(scene),
	m_sceneId	(sceneId),
	m_groupId	(0)
{ }


///////////////////////////////////////////////////////////
void EntityGroup::removeEntity(Entity::Id id)
{
	m_removeQueue.push_back(id);
}


///////////////////////////////////////////////////////////
void EntityGroup::removeQueuedEntities()
{
	// Call the remove function
	if (m_removeQueue.size())
	{
		m_removeFunc(m_removeQueue);
		m_removeQueue.clear();
	}
}


///////////////////////////////////////////////////////////
void EntityGroup::copyTypeSetTo(EntityGroup& group, Uint32 groupId)
{
	m_copyFunc(group, groupId);
}


///////////////////////////////////////////////////////////
Entity EntityGroup::moveEntity(Entity::Id id, EntityGroup& group)
{
	return m_moveFunc(id, group);
}


///////////////////////////////////////////////////////////
bool EntityGroup::hasComponentType(Uint32 type) const
{
	return m_componentTypes.find(type) != m_componentTypes.end();
}


///////////////////////////////////////////////////////////
void EntityGroup::addTag(Uint32 tag)
{
	m_tags.insert(tag);
}


///////////////////////////////////////////////////////////
bool EntityGroup::hasTag(Uint32 tag) const
{
	return m_tags.contains(tag);
}


///////////////////////////////////////////////////////////
std::vector<Entity::Id>& EntityGroup::getEntityIds()
{
	return m_entityIds.getData();
}


///////////////////////////////////////////////////////////
const HashSet<Uint32>& EntityGroup::getComponentTypes() const
{
	return m_componentTypes;
}


///////////////////////////////////////////////////////////
const HashSet<Uint32>& EntityGroup::getTags() const
{
	return m_tags;
}

}


///////////////////////////////////////////////////////////
ComponentTypeSet::ComponentTypeSet(const HashSet<Uint32>& set) :
	m_set	(set)
{

}


///////////////////////////////////////////////////////////
const HashSet<Uint32>& ComponentTypeSet::getSet() const
{
	return m_set;
}


///////////////////////////////////////////////////////////
TagSet::TagSet(const HashSet<Uint32>& set) :
	m_set	(set)
{

}


///////////////////////////////////////////////////////////
void TagSet::add(const std::string& tag)
{
	// Insert type id
	m_set.insert(std::hash<std::string>()(tag));
}


///////////////////////////////////////////////////////////
void TagSet::remove(const std::string& tag)
{
	// Remove type id
	m_set.erase(std::hash<std::string>()(tag));
}


///////////////////////////////////////////////////////////
bool TagSet::has(const std::string& tag) const
{
	return m_set.find(std::hash<std::string>()(tag)) != m_set.end();
}


///////////////////////////////////////////////////////////
const HashSet<Uint32>& TagSet::getSet() const
{
	return m_set;
}

}