#ifndef POLY_SCENE_H
#define POLY_SCENE_H

#include <poly/Core/Tuple.h>

#include <poly/Engine/Ecs.h>
#include <poly/Engine/Entity.h>

#include <poly/Graphics/VertexBuffer.h>

#include <unordered_map>

namespace poly
{

class Camera;
class Light;


///////////////////////////////////////////////////////////
/// \brief A class that represent a game scene and stores all data on
/// the current state of the game
///
///////////////////////////////////////////////////////////
class Scene
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// A new id is generated every time a scene is created.
	///
	///////////////////////////////////////////////////////////
	Scene();

	///////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	/// All existing entities and component data that belong to
	/// the scene are destroyed, and the scene's id is recycled
	/// for use by future scenes.
	///
	///////////////////////////////////////////////////////////
	~Scene();

#ifndef DOXYGEN_SKIP
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
#endif

	///////////////////////////////////////////////////////////
	/// \brief Get the scene id
	///
	/// Scene ids are generated when the scene is constructed
	/// and destroyed in its destructor.
	///
	/// \return The unique id
	///
	///////////////////////////////////////////////////////////
	Uint16 getId() const;

	///////////////////////////////////////////////////////////
	/// \brief Create a new entity with the specified component types
	///
	/// An entity with a unique id is created and returned. All
	/// componenents that are associated with the entity are initialized
	/// with the default constructor.
	///
	/// Accessing the component data using the returned Entity
	/// object is possible, but is slow. When processing large
	/// amounts of entities, use system() instead.
	///
	/// This function is thread-safe.
	///
	/// \tparam Cs The component types to attach to the entity
	///
	/// \return An Entity object with the specified components attached
	///
	///////////////////////////////////////////////////////////
	template <typename... Cs>
	Entity createEntity();

	///////////////////////////////////////////////////////////
	/// \brief Create a new entity with the specified component types
	///
	/// An entity with a unique id is created and returned. All
	/// componenents that are associated with the entity are initialized
	/// with the given values.
	///
	/// Accessing the component data using the returned Entity
	/// object is possible, but is slow. When processing large
	/// amounts of entities, use system() instead.
	///
	/// This function is thread-safe.
	///
	/// \tparam Cs The component types to attach to the entity
	///
	/// \param components The list of component data values (in order) to initialize the entity with
	///
	/// \return An Entity object with the specified components attached
	///
	///////////////////////////////////////////////////////////
	template <typename... Cs>
	Entity createEntity(Cs&&... components);


	///////////////////////////////////////////////////////////
	/// \brief Create a new entity with the specified component types
	///
	/// An entity with a unique id is created and returned. All
	/// componenents that are associated with the entity are initialized
	/// with the given values in the tuple.
	///
	/// Accessing the component data using the returned Entity
	/// object is possible, but is slow. When processing large
	/// amounts of entities, use system() instead.
	///
	/// This function is thread-safe.
	///
	/// \tparam Cs The component types to attach to the entity
	///
	/// \param components A tuple containing component data values (in order) to initialize the entity with
	///
	/// \return An Entity object with the specified components attached
	///
	///////////////////////////////////////////////////////////
	template <typename... Cs>
	Entity createEntity(Tuple<Cs...>& components);

	///////////////////////////////////////////////////////////
	/// \brief Create several entities with the specified component types
	///
	/// A list of entity with unique ids is created and returned. All
	/// componenents that are associated with the entity are initialized
	/// with the default constructor.
	///
	/// Accessing the component data using the returned Entity
	/// object is possible, but is slow. When processing large
	/// amounts of entities, use system() instead.
	///
	/// This function is thread-safe.
	///
	/// \tparam Cs The component types to attach to the entity
	///
	/// \param num The number of entities to create
	///
	/// \return A list of Entity objects with the specified components attached
	///
	///////////////////////////////////////////////////////////
	template <typename... Cs>
	std::vector<Entity> createEntities(Uint32 num);

	///////////////////////////////////////////////////////////
	/// \brief Create several entities with the specified component types
	///
	/// A list of entity with unique ids is created and returned. All
	/// componenents that are associated with the entities are initialized
	/// with the given values.
	///
	/// Accessing the component data using the returned Entity
	/// object is possible, but is slow. When processing large
	/// amounts of entities, use system() instead.
	///
	/// This function is thread-safe.
	///
	/// \tparam Cs The component types to attach to the entity
	///
	/// \param num The number of entities to create
	/// \param components The list of component data values (in order) to initialize the entities with
	///
	/// \return A list of Entity objects with the specified components attached
	///
	///////////////////////////////////////////////////////////
	template <typename... Cs>
	std::vector<Entity> createEntities(Uint32 num, Cs&&... components);

	///////////////////////////////////////////////////////////
	/// \brief Create several entities with the specified component types
	///
	/// A list of entity with unique ids is created and returned. All
	/// componenents that are associated with the entities are initialized
	/// with the given values in the tuple.
	///
	/// Accessing the component data using the returned Entity
	/// object is possible, but is slow. When processing large
	/// amounts of entities, use system() instead.
	///
	/// This function is thread-safe.
	///
	/// \tparam Cs The component types to attach to the entity
	///
	/// \param num The number of entities to create
	/// \param components A tuple containing component data values (in order) to initialize the entity with
	///
	/// \return A list of Entity objects with the specified components attached
	///
	///////////////////////////////////////////////////////////
	template <typename... Cs>
	std::vector<Entity> createEntities(Uint32 num, Tuple<Cs...>& components);

	///////////////////////////////////////////////////////////
	/// \brief Queue an entity for removal
	///
	/// Removing entities while component data is being processed
	/// can often lead to unpredictable behavior, so entities
	/// must be queued for removal instead.
	///
	/// The queued entities can then all be removed at once
	/// at a later time, most often at the end of the update frame.
	///
	/// This function is thread-safe.
	///
	/// \param id The id of the entity to remove
	///
	/// \see removeQueuedEntities
	///
	///////////////////////////////////////////////////////////
	void removeEntity(Entity::Id id);

	///////////////////////////////////////////////////////////
	/// \brief Remove all entities that are queued for removal
	///
	/// Removes all queued entities. This function should only be
	/// called once per frame. Most often, it should be called at
	/// the end of an update frame, after all component data has
	/// finished processing.
	///
	/// This function is thread-safe.
	///
	/// \see removeEntity
	///
	///////////////////////////////////////////////////////////
	void removeQueuedEntities();

	///////////////////////////////////////////////////////////
	/// \brief Get a pointer to component data associated with a certain entity
	///
	/// This function should not be used too often (it is slower
	/// compared to other functions when bulk access is needed).
	/// Use getComponentData() when bulk access is needed, or
	/// use system() when bulk processing of component data is
	/// needed.
	///
	/// \tparam C The component type to retrieve
	///
	/// \param id The id of the entity to retrieve a component for
	///
	/// \return A pointer to the requested component
	///
	///////////////////////////////////////////////////////////
	template <typename C>
	C* getComponent(Entity::Id id) const;

	///////////////////////////////////////////////////////////
	/// \brief Get component data for entities contain the specified component types
	///
	/// \tparam Cs The set of component types an entity must have to be
	/// included in the component arrays
	///
	/// \return A tuple containing a ComponentArray for each specified
	/// component type, as well as a ComponentArray of entity ids
	///
	///////////////////////////////////////////////////////////
	template <typename... Cs>
	Tuple<ComponentArray<Entity::Id>, ComponentArray<Cs>...> getComponentData();

	///////////////////////////////////////////////////////////
	/// \brief Get component data for entities contain the specified component types
	///
	/// Entities that contain component types that are included in the
	/// \a exclude component type set will not be included in
	/// the resulting component arrays.
	///
	/// \tparam Cs The set of component types an entity must have to be
	/// included in the component arrays
	///
	/// \param exclude The set of component types to exclude when matching entities
	///
	/// \return A tuple containing a ComponentArray for each specified
	/// component type, as well as a ComponentArray of entity ids
	///
	///////////////////////////////////////////////////////////
	template <typename... Cs>
	Tuple<ComponentArray<Entity::Id>, ComponentArray<Cs>...> getComponentData(const ComponentTypeSet& exclude);

	///////////////////////////////////////////////////////////
	/// \brief Add a tag to an entity
	///
	/// Tags should be an enum that can be cast to an integer.
	/// If entities need to be filtered by tags for processing
	/// by system(), use empty components as tags instead.
	///
	/// See the scene usage example for an example of empty components.
	///
	/// This function is thread-safe.
	///
	/// \param id The id of the entity to add a tag to
	/// \param tag The tag to add
	///
	///////////////////////////////////////////////////////////
	void addTag(Entity::Id id, int tag);

	///////////////////////////////////////////////////////////
	/// \brief Remove a tag from an entity
	///
	/// Tags should be an enum that can be cast to an integer.
	/// If entities need to be filtered by tags for processing
	/// by system(), use empty components as tags instead.
	///
	/// See the scene usage example for an example of empty components.
	///
	/// This function is thread-safe.
	///
	/// \param id The id of the entity to remove a tag from
	/// \param tag The tag to remove
	///
	///////////////////////////////////////////////////////////
	void removeTag(Entity::Id id, int tag);

	///////////////////////////////////////////////////////////
	/// \brief Check if an entity has a tag
	///
	/// Tags should be an enum that can be cast to an integer
	///
	/// This function is thread-safe.
	///
	/// \param id The id of the entity to check
	/// \param tag The tag to check for
	///
	/// \return True if the specified entity has the tag
	///
	///////////////////////////////////////////////////////////
	bool hasTag(Entity::Id id, int tag);

	///////////////////////////////////////////////////////////
	/// \brief Get entities that contain the specified tag
	///
	/// Tags should be an enum that can be cast to an integer
	///
	/// \param tag The tag to retrieve entities for
	///
	/// \return The set of entitties that contain the tag
	///
	///////////////////////////////////////////////////////////
	const HashSet<Entity::Id>& getEntitiesWithTag(Uint32 tag);

	///////////////////////////////////////////////////////////
	/// \brief Process or modify a set of component data
	///
	/// This is the "system" implementation of ECS. Functions that
	/// are passed into this will be executed once for every
	/// entity that match the required component types. Only entities
	/// that contain the specified component types and don't contain
	/// the specified exclude types will be included in this update.
	///
	/// The update function passed into this function should have
	/// the signature of:
	/// \code
	/// void update(const poly::Entity::Id& id, Cs&... components);
	/// \endcode
	///
	/// Where Cs&... is expanded for the specified component types.
	/// This function can be any type of callable, such as function
	/// pointers, functors, std::function, or lambdas. Lambda is
	/// recommended as it is the most performant, but any callable
	/// will work.
	///
	/// Every component parameter in the update function belongs
	/// to the given entity id. So it is possible to access
	/// components that are not specified in the required component
	/// types, but access to individual component data is much slower.
	///
	/// Usage example:
	/// \code
	///
	/// using namespace poly;
	///
	/// Scene scene;
	///
	/// // Create 100 entities with (int, float)
	/// scene.createEntities<int, float>(100, 314, 3.14f);
	///
	/// // int and float should be replaced with actual components
	///
	/// // Apply changes using a system
	/// scene.system<int, float>(
	///		// This system will target entities that contain both integer and float
	///		[](const Entity::Id& id, int& i, float& f)
	///		{
	///			++i;
	///			f += 1.0f;
	///		},
	///
	///		// But exclude booleans
	///		ComponentTypeSet::create<bool>()
	/// );
	///
	/// \endcode
	///
	/// \note Access to components during the update function is
	/// not protected by a mutex, so make sure code inside the update
	/// functions are thread-safe. This can be done by not accessing
	/// the same data at the same time from multiple threads, or by
	/// using mutexes (or other methods) to protect the shared data.
	///
	/// \tparam Cs The component types required for entities
	/// \tparam Func A callable type
	///
	/// \param func The update function
	/// \param excludes The set of component types to exclude
	///
	///////////////////////////////////////////////////////////
	template <typename... Cs, typename Func>
	void system(Func&& func, const ComponentTypeSet& excludes = ComponentTypeSet());

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
	/// Usage example:
	/// \code
	///
	/// using namespace poly;
	///
	/// // Events are structs that contain data about the event
	/// struct MsgEvent
	/// {
	///		std::string m_message;
	/// };
	///
	///
	/// // Add a listener using a function
	/// void listener1(const MsgEvent& event)
	/// {
	///		std::cout << "Listener 1: " << event.m_message << '\n';
	/// }
	///
	/// scene.addListener<MsgEvent>(listener1);
	///
	///
	/// // Add a listener using a member function
	/// class Listener
	/// {
	/// public:
	///		void onEvent(const MsgEvent& event)
	///		{
	///			std::cout << "Listener 2: " << event.m_message << '\n';
	///		}
	/// };
	///
	/// Listener listener;
	/// scene.addListener<MsgEvent>(std::bind(&Listener::onEvent, listener));
	///
	///
	/// // Add a listener using a lambda
	/// Handle listener = scene.addListener<MsgEvent>(
	///		[](const MsgEvent& event)
	///		{
	///			std::cout << "Listener 3: " << event.m_message << '\n';
	///		}
	/// );
	///
	/// \endcode
	///
	/// This function is thread-safe.
	///
	/// \tparam E The event type
	///
	/// \param func The listener function
	///
	/// \return A handle to the listener function, used to remove the listener
	///
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
	/// This function is thread-safe.
	///
	/// \tparam E The event type to remove a listener from
	///
	/// \param handle The handle of the listener to remove
	///
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
	/// This function is thread-safe.
	///
	/// \tparam E The event type to send
	///
	/// \param event The event data to send
	///
	///////////////////////////////////////////////////////////
	template <typename E>
	void sendEvent(const E& event);

	///////////////////////////////////////////////////////////
	/// \brief Add a light to the scene
	///
	/// The subtype of the light must be one of the predefine light types.
	/// Support for user-defined lights may be added in the future.
	///
	/// \param light A pointer to the light to add
	///
	///////////////////////////////////////////////////////////
	void addLight(Light* light);

	///////////////////////////////////////////////////////////
	/// \brief Remove a light from the scene
	///
	/// The subtype of the light must be one of the predefine light types.
	/// Support for user-defined lights may be added in the future.
	///
	/// \param light A pointer to the light to remove
	///
	///////////////////////////////////////////////////////////
	void removeLight(Light* light);

	///////////////////////////////////////////////////////////
	/// \brief Set the scene camera
	///
	/// \param camera A pointer to the camera
	///
	///////////////////////////////////////////////////////////
	void setCamera(Camera* camera);

	///////////////////////////////////////////////////////////
	/// \brief Get the scene camera
	///
	/// \return A pointer to the camera
	///
	///////////////////////////////////////////////////////////
	Camera* getCamera() const;

	///////////////////////////////////////////////////////////
	/// \brief Render the scene to a specified framebuffer
	///
	/// Still in progress... TODO
	///
	///////////////////////////////////////////////////////////
	void render();

private:
	Handle m_handle;									//!< The scene handle used for scene id

	HashMap<Uint32, priv::EntityGroup> m_entityGroups;	//!< Map of group id to priv::EntityGroup
	HashMap<int, HashSet<Entity::Id>> m_entityTags;		//!< Map of tags to sets of entity ids
	std::mutex m_entityMutex;							//!< Mutex to protect creation and removal of entities
	std::mutex m_tagMutex;								//!< Mutex to protect adding and removing tags

	Camera* m_camera;									//!< A pointer to the scene camera
	std::vector<DirectionLight*> m_dirLights;			//!< A list of directional lights
	VertexBuffer m_instanceBuffer;						//!< The instance buffer that stores instance transform data
	Uint32 m_instanceBufferOffset;						//!< The offset of the valid range of the instance buffer

	static HandleArray<bool> idArray;					//!< HandleArray to handle scene id generation
};

}

#include <poly/Engine/Scene.inl>

#endif

///////////////////////////////////////////////////////////
/// \class poly::Scene
/// \ingroup Engine
///
/// Scene is a class that manages all entities and components
/// of gameplay, as well as providing spatial structure and utilities.
///
/// The scene uses ECS (Entity-Component-System) to implement
/// gameplay logic. ECS is a design implementation for entities
/// (or game objects) and game logic that is ultimately meant
/// to improve performance of the game. It is built around the
/// principle of data oriented design (DOD), and storing and using
/// data in a cache friendly manner.
///
/// Modern CPUs have caches that can store smaller amounts of
/// data than the main memory, but is much faster to access.
/// Data that has been accessed recently is stored on the
/// cache, allowing it to be accessed again quickly. As new data
/// is brought in from the main memory, older data is removed from
/// the cache, so any requests to access the older data will
/// take much longer once it is no longer on the cache. The
/// main purpose of data-oriented designs is to maximize cache
/// usage. There are several things that can be done that achieve
/// this goal, but the most basics are: store and access data
/// sequentially (in terms of memory addresses and layout),
/// minimize pointer dereferences, etc.
///
/// ECS takes the idea that entities (game objects) by themselves
/// should be nothing special, just a container to add properties
/// and behavior to. Entities become unique when components are
/// added to them. For example a human NPC may have the Render, Transform,
/// Physics, and NPC components attached to their entity. A popular
/// example of a game engine that uses this model is Unity.
/// In ECS, which attempts to layout data in a cache friendly manner,
/// entities are just numerical ids that map to a set of components.
/// Components are just data, no functions or methods. In data-oriented
/// design, data and functionality are seperated. The way the component
/// data is stored is slightly different than the way the would
/// be stored in object-oriented design though. Instead of this:
///
/// \code
/// [[A, B, C], [A, B, C], [A, B, C], [A, B, C], [A, B, C]]
/// \endcode
///
/// They are stored like this:
///
/// \code
/// [A, A, A, A, A]
/// [B, B, B, B, B]
/// [C, C, C, C, C]
/// \endcode
///
/// This is done so that processing and modifying component data
/// can be done in a cache-friendly manner. Normally when applying
/// updates to functions, only a handful of components are actually
/// needed. So the required component lists are taken and iterated
/// over. This is done through the "S" of ECS, systems. Systems are
/// where all the actual logic and functionality goes. They iterate
/// through specific lists of components and apply a certain update to
/// each one. This is the main idea of ECS.
///
/// ECS and data-oriented design is a very complex topic
/// so it will not be fully explained here (because I don't
/// understand it either). If more information is needed, there
/// are several articles online that cover this topic thoroughly.
///
/// To create entities, use either createEntity() or createEntities().
/// To remove entities, use removeEntity(). Note that removeEntity()
/// only queues that entity for removal because often times, several
/// entities are removed in the middle of updating components, etc.
/// Removing them immediately could lead to unpredictable behavior
/// because other following systems might need the removed entities,
/// so entities must be queued for removal and must be removed
/// all at once using removeQueuedEntities(). This function should be
/// called once per frame.
///
/// To modify and update component data, use system().
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// Scene scene;
///
///
/// // ========================================================
/// // ECS
/// // ========================================================
///
/// // Create 100 entities with (int, float)
/// std::vector<Entity> entities = scene.createEntities<int, float>(100, 314, 3.14f);
///
/// // Accessing individual components
/// int* iPtr = entities[10].get<int>();					// By using the Entity object
/// iPtr = scene.getComponent<int>(entities[10].getId());	// or by using id
///
/// // Apply changes using a system
/// scene.system<int, float>(
///		// This system will target entities that contain both integer and float
///		[&](const Entity::Id& id, int& i, float& f)
///		{
///			++i;
///			f += 1.0f;
///
///			// Queue an entity for removal
///			scene.removeEntity(id);
///		},
///
///		// But exclude booleans
///		ComponentTypeSet::create<bool>()
/// );
///
/// // Remove all queued entities
/// scene.removeQueuedEntities();
///
///
/// // ========================================================
/// // Events
/// // ========================================================
///
/// // Events are structs that contain data about the event
/// struct MsgEvent
/// {
///		std::string m_message;
/// };
///
/// // Add an event listener (std::function)
/// Handle listener = scene.addListener<MsgEvent>(
///		[](const MsgEvent& event)
///		{
///			std::cout << "New message: " << event.m_message << '\n';
///		}
/// );
///
/// // Send an event
/// scene.sendEvent(MsgEvent{ "Hello World!" });
///
/// // Remove the listener
/// scene.removeListener<MsgEvent>(listener);
///
/// \endcode
///
///////////////////////////////////////////////////////////