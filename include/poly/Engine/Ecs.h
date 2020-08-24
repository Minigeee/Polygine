#ifndef POLY_ECS_H
#define POLY_ECS_H

#include <poly/Core/DataTypes.h>
#include <poly/Core/HandleArray.h>

#include <poly/Engine/Entity.h>

#include <functional>
#include <tuple>
#include <unordered_set>
#include <vector>

namespace poly
{

class Scene;

// Skip documentation for private
#ifndef DOXYGEN_SKIP

namespace priv
{

///////////////////////////////////////////////////////////

template <typename C>
class ComponentData
{
public:
	static void createComponents(Uint16 sceneId, Uint32 groupId, Uint16 num, const C& component);

	static void removeComponents(Uint16 sceneId, Uint32 groupId, const std::vector<Uint16>& indices);

	static C* getComponent(Uint16 sceneId, Uint32 groupId, Uint16 index);

	static std::vector<C>& getGroup(Uint16 sceneId, Uint32 groupId);

	static bool hasGroup(Uint16 sceneId, Uint32 groupId);

	static void cleanup(Uint16 sceneId);

private:
	typedef HashMap<Uint32, std::vector<C>> Data;

	static std::vector<Data> m_data;
};

///////////////////////////////////////////////////////////

class ComponentCleanup
{
public:
	template <typename C>
	static void registerType();

	static void cleanup(Uint16 sceneId);

private:
	static HashMap<Uint32, std::function<void(Uint16)>> m_cleanupFuncs;
};

///////////////////////////////////////////////////////////

class EntityGroup
{
public:
	EntityGroup();
	EntityGroup(Scene* scene, Uint16 sceneId);

	template <typename... Cs>
	std::vector<Entity> createEntities(Uint16 num, Cs&&... components);

	void removeEntity(Entity::Id id);

	void removeQueuedEntities();

	template <typename C>
	C* getComponent(Entity::Id id) const;

	template <typename C>
	std::vector<C>& getComponentData() const;

	template <typename... Cs>
	void setComponentTypes(Uint32 groupId);

	template <typename C>
	bool hasComponentType() const;

	bool hasComponentType(Uint32 type) const;

	std::vector<Entity::Id>& getEntityIds();

private:
	template <typename... Cs>
	void removeEntitiesImpl(const std::vector<Entity::Id>& ids);

private:
	Scene* m_scene;
	Uint16 m_sceneId;
	Uint32 m_groupId;

	HandleArray<Entity::Id> m_entityIds;
	HashSet<Uint32> m_componentTypes;
	std::vector<Entity::Id> m_removeQueue;

	std::function<void(const std::vector<Entity::Id>&)> m_removeFunc;
};

}

#endif

///////////////////////////////////////////////////////////
/// \brief Data structure class that holds references to component
/// data for several different entity groups
///
///////////////////////////////////////////////////////////
template <typename C>
class ComponentArray
{
public:
	///////////////////////////////////////////////////////////
	/// \brief A struct that is used to represent entity group data
	///
	///////////////////////////////////////////////////////////
	struct Group
	{
		///////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		///////////////////////////////////////////////////////////
		Group();

		///////////////////////////////////////////////////////////
		/// \brief Construct from a list of component data
		///
		///////////////////////////////////////////////////////////
		Group(std::vector<C>& data);


		C* m_data;		//!< A pointer to the start of the component data
		Uint16 m_size;	//!< The number of component objects in the data
	};

	///////////////////////////////////////////////////////////
	/// \brief A component array iterator
	///
	///////////////////////////////////////////////////////////
	struct Iterator
	{
	public:
		///////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		///////////////////////////////////////////////////////////
		Iterator();

		///////////////////////////////////////////////////////////
		/// \brief Construct from the parent component array
		///
		///////////////////////////////////////////////////////////
		Iterator(ComponentArray<C>* arr);

		///////////////////////////////////////////////////////////
		/// \brief Get the value referenced by the iterator
		///
		/// \return A reference to a component object
		///
		///////////////////////////////////////////////////////////
		C& get();

		///////////////////////////////////////////////////////////
		/// \brief Pre-increment operator
		///
		///////////////////////////////////////////////////////////
		Iterator& operator++();

		///////////////////////////////////////////////////////////
		/// \brief Post-increment operator
		///
		///////////////////////////////////////////////////////////
		Iterator operator++(int);

		///////////////////////////////////////////////////////////
		/// \brief Check if the iterator is at the end of the array
		///
		/// \return True if the iterator is currently on the last element in the array
		///
		///////////////////////////////////////////////////////////
		bool atEnd() const;

	private:
		ComponentArray<C>* m_array;	//!< A pointer to the parent component array

		C* m_ptr;					//!< A pointer to the current component object
		Uint32 m_group;				//!< The current group number
		Uint16 m_size;				//!< The size of the current group
		Uint16 m_index;				//!< The index in the current group
	};

public:
	///////////////////////////////////////////////////////////
	/// \brief Add a component group to the component array
	///
	/// A pointer to the first element of the vector is stored,
	/// so while the component array is being used, the list
	/// of component data representing the entity group must not
	/// be modified. Modifying the source data will potentially
	/// cause any pointer to the data to become invalidated.
	///
	/// \param group A list of component data
	///
	///////////////////////////////////////////////////////////
	void addGroup(std::vector<C>& group);

	///////////////////////////////////////////////////////////
	/// \brief Get a reference to a certain entity group
	///
	/// \param index The index of entity group to retrieve
	///
	/// \return A reference to the group at the specified index
	///
	///////////////////////////////////////////////////////////
	Group& getGroup(Uint32 index);

	///////////////////////////////////////////////////////////
	/// \brief Get the number of entity groups that have been added
	///
	/// \return The number of entity groups
	///
	///////////////////////////////////////////////////////////
	Uint32 getNumGroups() const;

	///////////////////////////////////////////////////////////
	/// \brief Get an iterator starting at the start of the array
	///
	/// \return An iterator pointing to the start of the array
	///
	///////////////////////////////////////////////////////////
	Iterator getIterator();

private:
	std::vector<Group> m_groups; //!< The list of entity groups
};


///////////////////////////////////////////////////////////
/// \brief A class that can store a set of data types
///
///////////////////////////////////////////////////////////
class ComponentTypeSet
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Create a component type set
	///
	/// \return A component type set containg the specified data types
	///
	///////////////////////////////////////////////////////////
	template <typename... Cs>
	static ComponentTypeSet create();

	///////////////////////////////////////////////////////////
	/// \brief Set the data types to include in the set
	///
	///////////////////////////////////////////////////////////
	template <typename... Cs>
	void set();

	///////////////////////////////////////////////////////////
	/// \brief Add data type to the set
	///
	///////////////////////////////////////////////////////////
	template <typename C>
	void add();

	///////////////////////////////////////////////////////////
	/// \brief Remove a data type from the set
	///
	///////////////////////////////////////////////////////////
	template <typename C>
	void remove();

	///////////////////////////////////////////////////////////
	/// \brief Check if a ceratin data type is in the set
	///
	/// \return True if the specified data type is in the set
	///
	///////////////////////////////////////////////////////////
	template <typename C>
	bool has() const;

	///////////////////////////////////////////////////////////
	/// \brief Get a hash set containing type ids of the component type set
	///
	/// \return A hash set
	///
	///////////////////////////////////////////////////////////
	const HashSet<Uint32>& getSet() const;

private:
	HashSet<Uint32> m_set; //!< Hash set of type ids
};

}

#include <poly/Engine/Ecs.inl>

#endif

///////////////////////////////////////////////////////////
/// \class poly::ComponentTypeSet
/// \ingroup Engine
///
/// ComponentTypeSet stores a set of data types (not values).
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// ComponentTypeSet c;
///
/// // Set the type set to have int, float, and bool
/// c.set<int, float, bool>();
///
/// // Add Uint32
/// c.add<Uint32>();
///
/// // Remove bool
/// c.remove<bool>();
///
/// c.has<int>();		// True
/// c.has<float>();		// True
/// c.has<bool>();		// False
/// c.has<Uint32>();	// True
///
/// \endcode
///
///////////////////////////////////////////////////////////