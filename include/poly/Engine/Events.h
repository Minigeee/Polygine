#ifndef POLY_EVENTS_H
#define POLY_EVENTS_H

#include <poly/Core/DataTypes.h>
#include <poly/Core/HandleArray.h>

#include <functional>
#include <mutex>
#include <vector>

namespace poly
{

#ifndef DOXYGEN_SKIP

namespace priv
{

template <typename E>
class EventSystem
{
public:
	static Handle addListener(Uint16 sceneId, std::function<void(const E&)>&& func);

	static void removeListener(Uint16 sceneId, Handle handle);

	static void sendEvent(Uint16 sceneId, const E& event);

private:
	static std::vector<HandleArray<std::function<void(const E&)>>> m_listeners;
	static std::mutex m_mutex;
};

}

#endif

}

#include <poly/Engine/Events.inl>

#endif