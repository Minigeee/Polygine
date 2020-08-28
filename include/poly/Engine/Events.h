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
/// \brief Handles events for a sent of event types
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

///////////////////////////////////////////////////////////
/// \class poly::EventSystem
/// \ingroup Engine
///
/// EventSystem handles adding listeners and sending events
/// for a limited set of event types. Only the event types
/// specified in the system's template parameters will be
/// valid. However, having a limited set of events will not
/// be a constraint most of the time, as there is only a limited
/// number of unique event types that can be created anyway.
///
/// This event system is meant to be convenient for the user
/// to setup, rather than being general purpose like the Scene
/// event system, which can handle any number of event types
/// without specifying which ones it can handle.
///
/// Use EventSystem::addListener() to add any listener functions.
/// This function returns a Handle to the listener function,
/// which can be used to remove the listener at a later time
/// if desired. EventSystem::sendEvent() will pass the given event
/// object to all existing listener functions.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// struct EventA { int m_a; };
/// struct EventB { float m_b; };
/// struct EventC { bool m_c; };
///
/// class A :
///		public EventSystem<EventA, EventB, EventC>
/// { };
///
/// // Create object
/// A a;
///
/// // Add a listener
/// Handle listener = a.addListener<EventA>(
///		[&](const EventA& e)
///		{
///			std::cout << e.m_a << '\n';
///		}
/// );
///
/// // Send a few events
/// a.sendEvent(EventA{ 1 });
/// a.sendEvent(EventA{ 2 });
/// a.sendEvent(EventA{ 3 });
///
/// // These won't do anything because there are no listeners for these events
/// a.sendEvent(EventB{ 3.14f });
/// a.sendEvent(EventB{ 6.28f });
///
/// // Remove the listener
/// a.removeListener<EventA>(listener);
///
/// \endcode
///
///////////////////////////////////////////////////////////