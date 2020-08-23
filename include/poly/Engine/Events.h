#ifndef POLY_EVENTS_H
#define POLY_EVENTS_H

#include <poly/Core/DataTypes.h>

#include <functional>
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
	static void addListener(Uint16 sceneId, std::function<void(const E&)>&& func);

	static void sendEvent(Uint16 sceneId, const E& event);

private:
	static std::vector<std::vector<std::function<void(const E&)>>> m_listeners;
};

}

#endif

}

#include <poly/Engine/Events.inl>

#endif