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

	// Get data iterators
	typename ComponentArray<Entity::Id>::Iterator entityIt = get<0>(data).getIterator();
	Tuple<typename ComponentArray<Cs>::Iterator...> its(data.get<ComponentArray<Cs>>().getIterator()...);

	// Iterate through all component data
	while (!entityIt.atEnd())
	{
		// Call the process function
		process(dt, entityIt.get(), its.get<typename ComponentArray<Cs>::Iterator>().get()...);

		// Increment iterators
		++entityIt;
		PARAM_EXPAND(++its.get<typename ComponentArray<Cs>::Iterator>());
	}
}

///////////////////////////////////////////////////////////

}