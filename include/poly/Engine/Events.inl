#include <poly/Core/TypeInfo.h>

namespace poly
{

namespace priv
{

template <typename E>
std::vector<HandleArray<std::function<void(const E&)>>> SceneEvents<E>::m_listeners;

template <typename E>
std::mutex SceneEvents<E>::m_mutex;

///////////////////////////////////////////////////////////

template <typename E>
inline Handle SceneEvents<E>::addListener(Uint16 sceneId, std::function<void(const E&)>&& func)
{
	static bool _init = (SceneEventsCleanup::registerType<E>(), true);

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
inline void SceneEvents<E>::removeListener(Uint16 sceneId, Handle handle)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	// Return if no listeners have been added
	if (sceneId >= m_listeners.size()) return;

	// Remove listener
	m_listeners[sceneId].remove(handle);
}

template <typename E>
inline void SceneEvents<E>::sendEvent(Uint16 sceneId, const E& event)
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
inline void SceneEvents<E>::cleanup(Uint16 sceneId)
{
	if (sceneId >= m_listeners.size() || !m_listeners[sceneId].size()) return;

	std::lock_guard<std::mutex> lock(m_mutex);

	// Clear the specified scene's listeners
	m_listeners[sceneId].reset();
}

///////////////////////////////////////////////////////////

template <typename E>
inline void SceneEventsCleanup::registerType()
{
	Uint32 typeId = TypeInfo::getId<E>();

	// Set the cleanup function
	if (m_cleanupFuncs.find(typeId) == m_cleanupFuncs.end())
		m_cleanupFuncs[typeId] = SceneEvents<E>::cleanup;
}

///////////////////////////////////////////////////////////

template <typename E>
inline Handle EventSystemImpl<E>::addListener(std::function<void(const E&)>&& func)
{
	// Add the listener and return the handle
	return m_listeners.add(std::move(func));
}

template <typename E>
inline void EventSystemImpl<E>::removeListener(Handle handle)
{
	m_listeners.remove(handle);
}

template <typename E>
inline void EventSystemImpl<E>::sendEvent(const E& event) const
{
	// Iterate all listeners and call the function
	const std::vector<std::function<void(const E&)>>& listeners = m_listeners.getData();
	for (Uint32 i = 0; i < listeners.size(); ++i)
		listeners[i](event);
}

///////////////////////////////////////////////////////////

}

template <typename... Es>
template <typename E>
Handle EventSystem<Es...>::addListener(std::function<void(const E&)>&& func)
{
	return priv::EventSystemImpl<E>::addListener(std::forward<std::function<void(const E&)>>(func));
}

template <typename... Es>
template <typename E>
void EventSystem<Es...>::removeListener(Handle handle)
{
	priv::EventSystemImpl<E>::removeListener(handle);
}

template <typename... Es>
template <typename E>
void EventSystem<Es...>::sendEvent(const E& event) const
{
	priv::EventSystemImpl<E>::sendEvent(event);
}

///////////////////////////////////////////////////////////

}