#include <poly/Core/Macros.h>
#include <poly/Core/TypeInfo.h>

#include <unordered_set>

namespace poly
{

namespace priv
{

///////////////////////////////////////////////////////////

template <typename... Cs>
inline Uint32 generateGroupId()
{
	// Create type set
	std::unordered_set<Uint32> types;
	PARAM_EXPAND(types.insert(TypeInfo::id<Cs>()));

	// Generate hash
	std::hash<Uint32> hasher;
	Uint32 hash = 1;

	for (auto& it = types.begin(); it != types.end(); ++it)
		// Use multiplication operator
		hash *= (Uint32)hasher(*it);

	return (Uint32)hasher(hash);
}

///////////////////////////////////////////////////////////

}

template <typename... Cs>
inline Entity Scene::createEntity()
{
	static_assert(sizeof...(Cs), "Entities must have at least one component type");
	return createEntity(Cs()...);
}

template <typename... Cs>
inline Entity Scene::createEntity(Cs&&... components)
{
	return createEntities(1, std::forward<Cs>(components)...)[0];
}

template <typename... Cs>
inline Entity Scene::createEntity(Tuple<Cs...>& components)
{
	return createEntity(std::forward<Cs>(components.get<Cs>())...);
}

template <typename... Cs>
inline std::vector<Entity> Scene::createEntities(Uint32 num)
{
	static_assert(sizeof...(Cs), "Entities must have at least one component type");
	return createEntities(num, Cs()...);
}

template <typename... Cs>
inline std::vector<Entity> Scene::createEntities(Uint32 num, Cs&&... components)
{
	static_assert(sizeof...(Cs), "Entities must have at least one component type");
	static_assert(priv::IsUnique<Cs...>::value, "Entities are not allowed to have duplicate component types");

	// Create a group id for this set
	static Uint32 groupId = priv::generateGroupId<Cs...>();

	// Find the correct group
	priv::EntityGroup* group = 0;

	auto it = m_entityGroups.find(groupId);
	if (it == m_entityGroups.end())
	{
		// Initialize group
		group = &(m_entityGroups[groupId] = priv::EntityGroup(this, m_handle.m_index));
		group->setComponentTypes<Cs...>(groupId);
	}
	else
		group = &it->second;

	// Create entity
	return group->createEntities(num, std::forward<Cs>(components)...);
}

template <typename... Cs>
inline std::vector<Entity> Scene::createEntities(Uint32 num, Tuple<Cs...>& components)
{
	return createEntities(num, std::forward<Cs>(components.get<Cs>())...);
}

template <typename C>
inline C* Scene::getComponent(Entity::Id id) const
{
	auto it = m_entityGroups.find(id.m_group);
	ASSERT(it != m_entityGroups.end(), "Group id for component type not found: %d", id.m_group);
	return it->second.getComponent<C>(id);
}

template <typename... Cs>
inline Tuple<ComponentArray<Entity::Id>, ComponentArray<Cs>...> Scene::getComponentData()
{
	Tuple<ComponentArray<Entity::Id>, ComponentArray<Cs>...> t;

	// Iterate all groups and find which ones contain the specified types
	for (auto it = m_entityGroups.begin(); it != m_entityGroups.end(); ++it)
	{
		priv::EntityGroup& group = it->second;

		// Test if it has the correct components
		bool matches = true;
		PARAM_EXPAND(matches &= group.hasComponentType<Cs>());

		// Add the group if it matches
		if (matches)
		{
			// Add each required component type
			PARAM_EXPAND(t.get<ComponentArray<Cs>>().addGroup(group.getComponentData<Cs>()));

			// Add entity ids
			t.get<ComponentArray<Entity::Id>>().addGroup(group.getEntityIds());
		}
	}

	// Return the tuple
	return t;
}

}