#ifndef POLY_SCENE_H
#define POLY_SCENE_H

#include <poly/Engine/Ecs.h>
#include <poly/Engine/Entity.h>

#include <unordered_map>

namespace poly
{

class Scene
{
public:
	Scene();

	template <typename... Cs>
	Entity createEntity();

	template <typename... Cs>
	Entity createEntity(Cs&&... components);

	template <typename... Cs>
	Entity createEntity(ComponentSet<Cs...>& components);

	template <typename T>
	T* getComponent(Entity::Id id) const;

private:
	Uint32 m_id;

	std::unordered_map<Uint32, priv::EntityGroup> m_entityGroups;
};

}

#include <poly/Engine/Scene.inl>

#endif