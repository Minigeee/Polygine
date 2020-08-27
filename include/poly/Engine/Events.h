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

	static void cleanup(Uint16 sceneId);

private:
	static std::vector<HandleArray<std::function<void(const E&)>>> m_listeners;
	static std::mutex m_mutex;
};


class EventCleanup
{
public:
	template <typename E>
	static void registerType();

	static void cleanup(Uint16 sceneId);

private:
	static HashMap<Uint32, std::function<void(Uint16)>> m_cleanupFuncs;
};

}

#endif

}

#include <poly/Engine/Events.inl>

#endif