#include <poly/Engine/Entity.h>
#include <poly/Engine/Scene.h>

namespace poly
{

///////////////////////////////////////////////////////////

Entity::Id::Id() :
	m_group		(0)
{ }

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

}