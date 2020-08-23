#include <poly/Engine/Entity.h>
#include <poly/Engine/Scene.h>

namespace poly
{

///////////////////////////////////////////////////////////

Entity::Id::Id() :
	m_group		(0)
{ }

bool Entity::Id::operator==(const Entity::Id& a) const
{
	return
		m_group == a.m_group &&
		m_handle.m_index == a.m_handle.m_index &&
		m_handle.m_counter == a.m_handle.m_counter;
}

Entity::Entity() :
	m_scene		(0)
{ }

Entity::Entity(Scene* scene, Id id) :
	m_scene		(scene),
	m_id		(id)
{ }

///////////////////////////////////////////////////////////

Entity::Id Entity::getId() const
{
	return m_id;
}

void Entity::remove() const
{
	m_scene->removeEntity(m_id);
}

///////////////////////////////////////////////////////////

void Entity::addTag(int tag)
{
	m_scene->addTag(m_id, tag);
}

void Entity::removeTag(int tag)
{
	m_scene->removeTag(m_id, tag);
}

bool Entity::hasTag(int tag) const
{
	return m_scene->hasTag(m_id, tag);
}

///////////////////////////////////////////////////////////

}