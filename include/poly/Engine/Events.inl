
namespace poly
{

namespace priv
{

template <typename E>
std::vector<std::vector<std::function<void(const E&)>>> EventSystem<E>::m_listeners;

///////////////////////////////////////////////////////////

template <typename E>
inline void EventSystem<E>::addListener(Uint16 sceneId, std::function<void(const E&)>&& func)
{
	// Create enough scene slots
	if (sceneId >= m_listeners.size())
	{
		while (sceneId >= m_listeners.size())
			m_listeners.push_back(std::vector<std::function<void(const E&)>>());
	}

	// Add listener
	m_listeners[sceneId].push_back(std::move(func));
}

template <typename E>
inline void EventSystem<E>::sendEvent(Uint16 sceneId, const E& event)
{
	// Return if no listeners have been added
	if (sceneId >= m_listeners.size()) return;

	const std::vector<std::function<void(const E&)>>& listeners = m_listeners[sceneId];

	// Call all listeners
	for (Uint32 i = 0; i < listeners.size(); ++i)
		listeners[i](event);
}

///////////////////////////////////////////////////////////

}

}