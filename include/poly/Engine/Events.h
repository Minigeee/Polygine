#ifndef POLY_EVENTS_H
#define POLY_EVENTS_H

#include <poly/Core/DataTypes.h>
#include <poly/Core/HandleArray.h>

#include <functional>
#include <mutex>
#include <vector>

namespace poly
{

///////////////////////////////////////////////////////////

#ifndef DOXYGEN_SKIP

namespace priv
{

template <typename E>
class SceneEvents
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


class SceneEventsCleanup
{
public:
	template <typename E>
	static void registerType();

	static void cleanup(Uint16 sceneId);

private:
	static HashMap<Uint32, std::function<void(Uint16)>> m_cleanupFuncs;
};

template <typename E>
class EventSystemImpl
{
protected:
	Handle addListener(std::function<void(const E&)>&& func);

	void removeListener(Handle handle);

	void sendEvent(const E& event) const;

private:
	HandleArray<std::function<void(const E&)>> m_listeners;
};

}

#endif

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
/// \brief Handle events for a sent of event types
///
///////////////////////////////////////////////////////////
template <typename... Es>
class EventSystem :
	public priv::EventSystemImpl<Es>...
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Add an event listener function
	///
	/// The given function will be called every time the specified
	/// event is sent, and it must be casted into a std::function.
	/// This means the function can be a function pointer, a member
	/// function pointer, lambda, or any other function created
	/// with std::bind.
	///
	/// A handle to the event listener is returned in case the
	/// listener needs to be removed later.
	///
	/// Functions similar to Scene::addListener()
	///
	/// \param func The listener function
	///
	/// \return A handle to the listener function, used to remove the listener
	///
	/// \see Scene::addListener
	/// \see removeListener
	///
	///////////////////////////////////////////////////////////
	template <typename E>
	Handle addListener(std::function<void(const E&)>&& func);

	///////////////////////////////////////////////////////////
	/// \brief Remove an event listener function
	///
	/// The handle must be acquired from addListener().
	///
	/// Functions similar to Scene::removeListener()
	///
	/// \param handle The handle of the listener to remove
	///
	/// \see Scene::removeListener
	/// \see addListener
	///
	///////////////////////////////////////////////////////////
	template <typename E>
	void removeListener(Handle handle);

	///////////////////////////////////////////////////////////
	/// \brief Send an event to all registered event listener functions
	///
	/// All listener functions added under the specified event type
	/// will be invoked, but not necessarily in the order the
	/// listeners were added.
	///
	/// Functions similar to Scene::sendEvent()
	///
	/// \param event The event data to send
	///
	/// \see Scene::sendEvent
	///
	///////////////////////////////////////////////////////////
	template <typename E>
	void sendEvent(const E& event) const;
};

///////////////////////////////////////////////////////////

}

#include <poly/Engine/Events.inl>

#endif