#include <poly/Engine/Events.h>

namespace poly
{

namespace priv
{

HashMap<Uint32, std::function<void(Uint16)>> SceneEventsCleanup::m_cleanupFuncs;

///////////////////////////////////////////////////////////

void SceneEventsCleanup::cleanup(Uint16 sceneId)
{
	// Iterate hash map and call all cleanup functions
	for (auto it = m_cleanupFuncs.begin(); it != m_cleanupFuncs.end(); ++it)
		it.value()(sceneId);
}

///////////////////////////////////////////////////////////

}

}