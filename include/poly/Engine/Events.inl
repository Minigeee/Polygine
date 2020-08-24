#include <poly/Core/TypeInfo.h>

namespace poly
{

namespace priv
{

template <typename E>
std::vector<HandleArray<std::function<void(const E&)>>> EventSystem<E>::m_listeners;

template <typename E>
std::mutex EventSystem<E>::m_mutex;

///////////////////////////////////////////////////////////

template <typename E>
inline Handle EventSystem<E>::addListener(Uint16 sceneId, std::function<void(const E&)>&& func)
{
	static bool _init = (EventCleanup::registerType<E>(), true);

	std::lock_guard<std::mutex> lock(m_mutex);

	// Create enough scene slots
	if (sceneId >= m_listeners.size())
	{
		while (sceneId >= m_listeners.size())
			m_listeners.push_back(HandleArray<std::function<void(const E&)>>());
	}

	// Add listener
	return m_listeners[sceneId].add(std::move(func));
}

template <typename E>
inline void EventSystem<E>::removeListener(Uint16 sceneId, Handle handle)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	// Return if no listeners have been added
	if (sceneId >= m_listeners.size()) return;

	// Remove listener
	m_listeners[sceneId].remove(handle);
}

template <typename E>
inline void EventSystem<E>::sendEvent(Uint16 sceneId, const E& event)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	// Return if no listeners have been added
	if (sceneId >= m_listeners.size()) return;

	const std::vector<std::function<void(const E&)>>& listeners = m_listeners[sceneId].getData();

	// Call all listeners
	for (Uint32 i = 0; i < listeners.size(); ++i)
		listeners[i](event);
}

template <typename E>
inline void EventSystem<E>::cleanup(Uint16 sceneId)
{
	if (sceneId >= m_listeners.size() || !m_listeners[sceneId].size()) return;

	std::lock_guard<std::mutex> lock(m_mutex);

	// Clear the specified scene's listeners
	m_listeners[sceneId].reset();
}

///////////////////////////////////////////////////////////

template <typename E>
inline void EventCleanup::registerType()
{
	Uint32 typeId = TypeInfo::id<E>();

	// Set the cleanup function
	if (m_cleanupFuncs.find(typeId) == m_cleanupFuncs.end())
		m_cleanupFuncs[typeId] = EventSystem<E>::cleanup;
}

///////////////////////////////////////////////////////////

}

}