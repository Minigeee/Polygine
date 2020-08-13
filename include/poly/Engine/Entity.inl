#include <poly/Engine/Ecs.h>

namespace poly
{

template <typename C>
inline C* Entity::get() const
{
	return m_scene->getComponent<C>(m_id);
}

template <typename C>
inline bool Entity::has() const
{
	return priv::ComponentData<C>::hasGroup(m_scene->getId(), m_id.m_group);
}

}