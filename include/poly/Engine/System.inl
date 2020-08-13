#include <poly/Core/Profiler.h>

namespace poly
{

///////////////////////////////////////////////////////////

template <typename... Cs>
inline System<Cs...>::System() :
	m_scene		(0)
{ }

template <typename... Cs>
inline System<Cs...>::System(Scene* scene) :
	m_scene		(scene)
{ }

///////////////////////////////////////////////////////////

template <typename... Cs>
inline void System<Cs...>::update(float dt)
{
	// Get component data
	Tuple<ComponentArray<Entity::Id>, ComponentArray<Cs>...> data = m_scene->getComponentData<Cs...>();

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
			process(dt, idPtr[n], ptrs.get<Cs*>()[n]...);
	}
}

///////////////////////////////////////////////////////////

}