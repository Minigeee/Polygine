#include <poly/Core/Macros.h>
#include <poly/Core/TypeInfo.h>

#include <poly/Engine/Events.h>

#include <mutex>
#include <unordered_set>

#ifndef _COMPONENT_DECAY
#define _COMPONENT_DECAY(Cs) typename std::remove_pointer<typename std::decay<Cs>::type>::type
#endif

namespace poly
{

namespace priv
{


///////////////////////////////////////////////////////////
template <typename... Cs>
inline Uint32 generateGroupId()
{
	// Create type set
	HashSet<Uint32> types;
	PARAM_EXPAND(types.insert(TypeInfo::getId<Cs>()));

	// Generate hash
	std::hash<Uint32> hasher;
	Uint32 hash = 1;

	for (auto& it = types.begin(); it != types.end(); ++it)
		// Use multiplication operator
		hash *= (Uint32)hasher(*it);

	return (Uint32)hasher(hash);
}

}


///////////////////////////////////////////////////////////
template <typename... Cs>
inline Entity Scene::createEntity()
{
	static_assert(sizeof...(Cs), "Entities must have at least one component type");
	return createEntity(Cs()...);
}


///////////////////////////////////////////////////////////
template <typename... Cs>
inline Entity Scene::createEntity(Cs&&... components)
{
	return createEntities(1, std::forward<Cs>(components)...)[0];
}


///////////////////////////////////////////////////////////
template <typename... Cs>
inline Entity Scene::createEntity(Tuple<Cs...>& components)
{
	return createEntity(std::forward<Cs>(components.get<Cs>())...);
}


///////////////////////////////////////////////////////////
template <typename... Cs>
inline std::vector<Entity> Scene::createEntities(Uint32 num)
{
	static_assert(sizeof...(Cs), "Entities must have at least one component type");
	return createEntities(num, Cs()...);
}


///////////////////////////////////////////////////////////
template <typename... Cs>
inline std::vector<Entity> Scene::createEntities(Uint32 num, Cs&&... components)
{
	static_assert(sizeof...(Cs), "Entities must have at least one component type");
	static_assert(priv::IsUnique<_COMPONENT_DECAY(Cs)...>::value, "Entities are not allowed to have duplicate component types");

	// Create a group id for this set
	static Uint32 groupId = priv::generateGroupId<_COMPONENT_DECAY(Cs)...>();

	// Find the correct group
	priv::EntityGroup* group = 0;
	std::vector<Entity> entities;
	{
		// Lock component mutexes
		std::unique_lock<std::mutex> locks[] = {
			std::unique_lock<std::mutex>(priv::ComponentMutex<_COMPONENT_DECAY(Cs)>::s_mutex)...
		};

		// Entity creation is protected by mutex
		std::lock_guard<std::mutex> lock(m_entityMutex);

		auto it = m_entityGroups.find(groupId);
		if (it == m_entityGroups.end())
		{
			// Initialize group
			group = &(m_entityGroups[groupId] = priv::EntityGroup(this, m_handle.m_index));
			group->setComponentTypes<_COMPONENT_DECAY(Cs)...>(groupId);
		}
		else
			group = &it.value();

		// Create entity
		entities = std::move(group->createEntities(num, std::forward<Cs>(components)...));
	}

	// Send the event
	sendEvent(E_EntitiesCreated(entities));

	return entities;
}


///////////////////////////////////////////////////////////
template <typename... Cs>
inline std::vector<Entity> Scene::createEntities(Uint32 num, Tuple<Cs...>& components)
{
	return createEntities(num, std::forward<Cs>(components.get<Cs>())...);
}


///////////////////////////////////////////////////////////
template <typename C>
inline C* Scene::getComponent(Entity::Id id)
{
	std::lock_guard<std::mutex> lock(m_entityMutex);

	auto it = m_entityGroups.find(id.m_group);
	return it == m_entityGroups.end() ? 0 : it.value().getComponent<C>(id);
}


///////////////////////////////////////////////////////////
template <typename... Cs>
inline Tuple<Cs*...> Scene::getComponents(Entity::Id id)
{
	std::lock_guard<std::mutex> lock(m_entityMutex);

	auto it = m_entityGroups.find(id.m_group);
	if (it == m_entityGroups.end())
		return makeTuple((Cs*)(0)...);
	else
		return makeTuple(it.value().getComponent<Cs>(id)...);
}


///////////////////////////////////////////////////////////
template <typename... Cs>
inline Tuple<ComponentArray<Entity::Id>, ComponentArray<Cs>...> Scene::getComponentData()
{
	Tuple<ComponentArray<Entity::Id>, ComponentArray<Cs>...> t;

	std::lock_guard<std::mutex> lock(m_entityMutex);

	// Iterate all groups and find which ones contain the specified types
	for (auto it = m_entityGroups.begin(); it != m_entityGroups.end(); ++it)
	{
		priv::EntityGroup& group = it.value();

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


///////////////////////////////////////////////////////////
template <typename... Cs>
inline Tuple<ComponentArray<Entity::Id>, ComponentArray<Cs>...> Scene::getComponentData(const ComponentTypeSet& exclude)
{
	Tuple<ComponentArray<Entity::Id>, ComponentArray<Cs>...> t;

	std::lock_guard<std::mutex> lock(m_entityMutex);

	// Iterate all groups and find which ones contain the specified types
	for (auto it = m_entityGroups.begin(); it != m_entityGroups.end(); ++it)
	{
		priv::EntityGroup& group = it.value();

		// Test if it has the correct components
		bool matches = true;
		PARAM_EXPAND(matches &= group.hasComponentType<Cs>());

		// Check exclusions
		const HashSet<Uint32>& excludeSet = exclude.getSet();
		if (excludeSet.size())
		{
			for (auto it = excludeSet.begin(); it != excludeSet.end(); ++it)
				matches &= !group.hasComponentType(*it);
		}

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


///////////////////////////////////////////////////////////
template <typename... Cs, typename Func>
inline void Scene::system(Func&& func, const ComponentTypeSet& excludes)
{
	std::unique_lock<std::mutex> locks[] = { std::unique_lock<std::mutex>(priv::ComponentMutex<Cs>::s_mutex)... };

	// Get component data
	Tuple<ComponentArray<Entity::Id>, ComponentArray<Cs>...> data = getComponentData<Cs...>(excludes);

	// Get component info
	typename ComponentArray<Entity::Id>& entityArray = get<0>(data);
	Uint32 numGroups = entityArray.getNumGroups();

	// Iterate through groups
	for (Uint32 i = 0; i < numGroups; ++i)
	{
		// Group size
		Uint16 size = entityArray.getGroup(i).m_size;

		// Data pointers
		Entity::Id* idPtr = entityArray.getGroup(i).m_data;

		// For some reason tuple constructor with parameter pack doesn't work
		// so using the set function
		Tuple<Cs*...> ptrs;
		PARAM_EXPAND(ptrs.set<Cs*>(data.get<ComponentArray<Cs>>().getGroup(i).m_data));

		// Process all data in the group
		for (Uint16 n = 0; n < size; ++n)
			func(idPtr[n], ptrs.get<Cs*>()[n]...);
	}
}


///////////////////////////////////////////////////////////
template <typename E>
inline Handle Scene::addListener(std::function<void(const E&)>&& func)
{
	return priv::SceneEvents<E>::addListener(m_handle.m_index, std::move(func));
}


///////////////////////////////////////////////////////////
template <typename E>
inline void Scene::removeListener(Handle handle)
{
	priv::SceneEvents<E>::removeListener(m_handle.m_index, handle);
}


///////////////////////////////////////////////////////////
template <typename E>
inline void Scene::sendEvent(const E& event)
{
	priv::SceneEvents<E>::sendEvent(m_handle.m_index, event);
}


///////////////////////////////////////////////////////////
template <typename T>
inline T* Scene::getExtension()
{
	auto it = m_extensions.find(TypeInfo::getId<T>());
	if (it == m_extensions.end())
	{
		// Create the extension if it doesn't exist
		return (T*)(m_extensions[TypeInfo::getId<T>()] = new T(this));
	}

	return (T*)it.value();
}


}