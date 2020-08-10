#include <poly/Core/Macros.h>
#include <poly/Core/TypeInfo.h>

namespace poly
{

namespace priv
{

///////////////////////////////////////////////////////////

template <typename... Cs>
inline void EntityGroup::setComponentTypes(Uint32 groupId)
{
	// Set group id
	m_groupId = groupId;

	// Add component types
	PARAM_EXPAND(m_componentTypes.insert(TypeInfo::id<Cs>()));

	// Set remove function
	m_removeFunc = std::bind(&EntityGroup::removeEntitiesImpl<Cs...>, this, std::placeholders::_1);
}

template <typename... Cs>
inline std::vector<Entity> EntityGroup::createEntities(Uint16 num, Cs&&... components)
{
	// Add components
	PARAM_EXPAND(ComponentData<Cs>::createComponents(m_sceneId, m_groupId, num, std::forward<Cs>(components)));

	// Create entities
	std::vector<Entity> entities;
	entities.reserve(num);

	for (Uint16 i = 0; i < num; ++i)
	{
		// Create entity
		Handle handle = m_entityIds.add(Entity::Id());

		Entity::Id& id = m_entityIds[handle];
		id.m_handle = handle;
		id.m_group = m_groupId;

		// Add to return list
		entities.push_back(Entity(id));
	}

	return entities;
}

template <typename... Cs>
inline void EntityGroup::removeEntitiesImpl(const std::vector<Entity::Id>& ids)
{
	// To remove entities, have to remove components at certain indices
	// Handle array removes using swap-pop, so index of components will change over time
	// while removing

	// So need to keep track of component indices
	std::vector<Uint16> indices;

	for (Uint32 i = 0; i < ids.size(); ++i)
	{
		Handle handle = ids[i].m_handle;

		// Add the component index to the list
		indices.push_back(m_entityIds.getIndex(handle));

		// Then remove target entity
		m_entityIds.remove(handle);
	}

	// Remove components
	PARAM_EXPAND(ComponentData<Cs>::removeComponents(m_sceneId, m_groupId, indices));
}

template <typename C>
inline C* EntityGroup::getComponent(Entity::Id id) const
{
	return ComponentData<C>::getComponent(m_sceneId, m_groupId, m_entityIds.getIndex(id.m_handle));
}

template <typename C>
inline std::vector<C>& EntityGroup::getComponentData() const
{
	return ComponentData<C>::getGroup(m_sceneId, m_groupId);
}

template <typename C>
inline bool EntityGroup::hasComponentType() const
{
	return m_componentTypes.find(TypeInfo::id<C>()) != m_componentTypes.end();
}

///////////////////////////////////////////////////////////

template <typename C>
inline void ComponentData<C>::createComponents(Uint16 sceneId, Uint32 groupId, Uint16 num, const C& component)
{
	// Create enough scene slots
	if (sceneId >= m_data.size())
	{
		while (sceneId >= m_data.size())
			m_data.push_back(Data());
	}

	// Get the correct group
	std::vector<C>& group = m_data[sceneId][groupId];

	// Add components
	for (Uint16 i = 0; i < num; ++i)
		group.push_back(component);
}

template <typename C>
inline void ComponentData<C>::removeComponents(Uint16 sceneId, Uint32 groupId, const std::vector<Uint16>& indices)
{
	ASSERT(sceneId < m_data.size(), "Scene id does not exist for component type: %d", sceneId);

	// Get group
	std::vector<C>& group = m_data[sceneId][groupId];

	// Remove components by index
	for (Uint32 i = 0; i < indices.size(); ++i)
	{
		// By using swap-pop
		group[indices[i]] = group.back();
		group.pop_back();
	}
}

template <typename C>
inline C* ComponentData<C>::getComponent(Uint16 sceneId, Uint32 groupId, Uint16 index)
{
	ASSERT(sceneId < m_data.size(), "Scene id does not exist for component type: %d", sceneId);

	std::vector<C>& group = m_data[sceneId][groupId];
	ASSERT(index < group.size(), "Component index out of bounds: %d", index);

	return &group[index];
}

template <typename C>
inline std::vector<C>& ComponentData<C>::getGroup(Uint16 sceneId, Uint32 groupId)
{
	ASSERT(sceneId < m_data.size(), "Scene id does not exist for component type: %d", sceneId);
	return m_data[sceneId][groupId];
}

///////////////////////////////////////////////////////////

}

template <typename... Cs>
template <typename C>
inline void ComponentSet<Cs...>::set(const C& component)
{
	static_cast<priv::ComponentSetData<C>*>(this)->m_data = component;
}

template <typename... Cs>
template <typename C>
inline C& ComponentSet<Cs...>::get()
{
	return static_cast<priv::ComponentSetData<C>*>(this)->m_data;
}

}