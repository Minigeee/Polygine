#ifndef POLY_SCENE_H
#define POLY_SCENE_H

#include <poly/Core/Tuple.h>
#include <poly/Engine/Ecs.h>
#include <poly/Engine/Entity.h>

#include <unordered_map>

namespace poly
{

class Scene
{
public:
	Scene();

	Uint16 getId() const;

	template <typename... Cs>
	Entity createEntity();

	template <typename... Cs>
	Entity createEntity(Cs&&... components);

	template <typename... Cs>
	Entity createEntity(Tuple<Cs...>& components);

	template <typename... Cs>
	std::vector<Entity> createEntities(Uint32 num);

	template <typename... Cs>
	std::vector<Entity> createEntities(Uint32 num, Cs&&... components);

	template <typename... Cs>
	std::vector<Entity> createEntities(Uint32 num, Tuple<Cs...>& components);

	void removeEntity(Entity::Id id);

	void removeQueuedEntities();

	template <typename C>
	C* getComponent(Entity::Id id) const;

	template <typename... Cs>
	Tuple<ComponentArray<Entity::Id>, ComponentArray<Cs>...> getComponentData();

private:
	/* TODO : Scene id system */
	Uint16 m_id;

	std::unordered_map<Uint32, priv::EntityGroup> m_entityGroups;
};

}

#include <poly/Engine/Scene.inl>

#endif