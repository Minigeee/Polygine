#include <poly/Core/Macros.h>
#include <poly/Core/TypeInfo.h>

#include <unordered_set>

namespace poly
{

namespace priv
{

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

}

template <typename... Cs>
inline Entity Scene::createEntity(Cs&&... components)
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
		group = &(m_entityGroups[groupId] = priv::EntityGroup(m_id));
		group->setComponentTypes<Cs...>(groupId);
	}
	else
		group = &it->second;

	// Create entity
	return group->createEntities(1, std::forward<Cs>(components)...)[0];
}

template <typename... Cs>
inline Entity Scene::createEntity()
{
	static_assert(sizeof...(Cs), "Entities must have at least one component type");
	return createEntity(Cs()...);
}

template <typename... Cs>
inline Entity Scene::createEntity(ComponentSet<Cs...>& components)
{
	return createEntity(std::forward<Cs>(components.get<Cs>())...);
}

}