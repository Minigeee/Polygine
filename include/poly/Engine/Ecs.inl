#include <poly/Core/Logger.h>
#include <poly/Core/Macros.h>
#include <poly/Core/TypeInfo.h>

namespace poly
{

namespace priv
{

template <typename C>
std::vector<typename ComponentData<C>::Data> ComponentData<C>::m_data;

///////////////////////////////////////////////////////////

template <typename... Cs>
inline void EntityGroup::setComponentTypes(Uint32 groupId)
{
	// Set group id
	m_groupId = groupId;

	// Add component types
	PARAM_EXPAND(m_componentTypes.insert(TypeInfo::getId<Cs>()));

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
		entities.push_back(Entity(m_scene, id));
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
	return m_componentTypes.find(TypeInfo::getId<C>()) != m_componentTypes.end();
}

///////////////////////////////////////////////////////////

template <typename C>
inline void ComponentData<C>::createComponents(Uint16 sceneId, Uint32 groupId, Uint16 num, const C& component)
{
	// Initialize cleanup
	static bool _init = (ComponentCleanup::registerType<C>(), true);

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
	if (sceneId >= m_data.size()) return 0;

	// Return ptr to component
	std::vector<C>& group = m_data[sceneId][groupId];
	return index < group.size() ? &group[index] : 0;
}

template <typename C>
inline std::vector<C>& ComponentData<C>::getGroup(Uint16 sceneId, Uint32 groupId)
{
	ASSERT(sceneId < m_data.size(), "Scene id does not exist for component type: %d", sceneId);
	return m_data[sceneId][groupId];
}

template <typename C>
inline bool ComponentData<C>::hasGroup(Uint16 sceneId, Uint32 groupId)
{
	// If the scene doesn't exist for this component, then the group doesn't either
	if (sceneId >= m_data.size()) return false;

	const Data& data = m_data[sceneId];
	return data.find(groupId) != data.end();
}

template <typename C>
inline void ComponentData<C>::cleanup(Uint16 sceneId)
{
	if (sceneId >= m_data.size() || !m_data[sceneId].size()) return;

	// Reset scene data
	m_data[sceneId] = Data();
}

///////////////////////////////////////////////////////////

template <typename C>
inline void ComponentCleanup::registerType()
{
	Uint32 typeId = TypeInfo::getId<C>();

	// Set the cleanup function
	if (m_cleanupFuncs.find(typeId) == m_cleanupFuncs.end())
		m_cleanupFuncs[typeId] = ComponentData<C>::cleanup;
}

///////////////////////////////////////////////////////////

}

template <typename C>
inline ComponentArray<C>::Group::Group() :
	m_data		(0),
	m_size		(0)
{

}

template <typename C>
inline ComponentArray<C>::Group::Group(std::vector<C>& data) :
	m_data		(0),
	m_size		((Uint16)data.size())
{
	if (m_size)
		m_data = &data[0];
}

template <typename C>
inline ComponentArray<C>::Iterator::Iterator() :
	m_array		(0),
	m_ptr		(0),
	m_group		(0),
	m_size		(0),
	m_index		(0)
{

}

template <typename C>
inline ComponentArray<C>::Iterator::Iterator(ComponentArray<C>* arr) :
	m_array		(arr),
	m_ptr		(0),
	m_group		(0),
	m_size		(0),
	m_index		(0)
{
	if (m_array)
	{
		const Group& group = m_array->m_groups[0];
		m_size = group.m_size;
		m_ptr = group.m_data;
	}
}

template <typename C>
inline C& ComponentArray<C>::Iterator::get()
{
	ASSERT(m_index < m_size, "Component array iterator is out of bounds");
	return *m_ptr;
}

template <typename C>
inline typename ComponentArray<C>::Iterator& ComponentArray<C>::Iterator::operator++()
{
	ASSERT(m_array, "Component array iterator is not initialized");

	// Check to see if at end of current group
	if (++m_index == m_size && m_group + 1 < m_array->m_groups.size())
	{
		// Switch to next group
		const Group& group = m_array->m_groups[++m_group];
		m_size = group.m_size;
		m_ptr = group.m_data;
		m_index = 0;
	}
	else
		// Otherwise, iterate pointer
		++m_ptr;

	return *this;
}

template <typename C>
inline typename ComponentArray<C>::Iterator ComponentArray<C>::Iterator::operator++(int)
{
	// Store old version
	Iterator it = *this;
	operator++();
	return it;
}

template <typename C>
inline bool ComponentArray<C>::Iterator::atEnd() const
{
	ASSERT(m_array, "Component iterator is not initialized");
	return m_index >= m_size && m_group + 1 >= m_array->m_groups.size();
}

///////////////////////////////////////////////////////////

template <typename C>
inline void ComponentArray<C>::addGroup(std::vector<C>& group)
{
	m_groups.push_back(Group(group));
}

template <typename C>
inline typename ComponentArray<C>::Group& ComponentArray<C>::getGroup(Uint32 index)
{
	return m_groups[index];
}

template <typename C>
inline Uint32 ComponentArray<C>::getNumGroups() const
{
	return m_groups.size();
}

template <typename C>
inline typename ComponentArray<C>::Iterator ComponentArray<C>::getIterator()
{
	return Iterator(this);
}

///////////////////////////////////////////////////////////

template <typename... Cs>
inline ComponentTypeSet ComponentTypeSet::create()
{
	ComponentTypeSet set;
	set.set<Cs...>();
	return set;
}

template <typename... Cs>
inline void ComponentTypeSet::set()
{
	// Clear the set
	m_set.clear();

	// Insert all type ids
	PARAM_EXPAND(m_set.insert(TypeInfo::getId<Cs>()));
}

template <typename C>
inline void ComponentTypeSet::add()
{
	// Insert type id
	m_set.insert(TypeInfo::getId<C>());
}

template <typename C>
inline void ComponentTypeSet::remove()
{
	// Remove type id
	m_set.erase(TypeInfo::getId<C>());
}

template <typename C>
inline bool ComponentTypeSet::has() const
{
	return m_set.find(TypeInfo::getId<C>()) != m_set.end();
}

}