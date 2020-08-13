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
	~Scene();

#ifndef DOXYGEN_SKIP
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
#endif

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
	Handle m_handle;

	std::unordered_map<Uint32, priv::EntityGroup> m_entityGroups;

	static HandleArray<bool> idArray;
};

}

#include <poly/Engine/Scene.inl>

#endif