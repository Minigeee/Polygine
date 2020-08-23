#ifndef POLY_ENTITY_H
#define POLY_ENTITY_H

#include <poly/Core/HandleArray.h>

namespace poly
{

class Scene;

///////////////////////////////////////////////////////////
/// \brief A convenience wrapper class for entity ids
///
///////////////////////////////////////////////////////////
class Entity
{
public:
	///////////////////////////////////////////////////////////
	/// \brief An id used for entity and component operations
	///
	///////////////////////////////////////////////////////////
	struct Id
	{
		///////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		///////////////////////////////////////////////////////////
		Id();

		Handle m_handle;	//!< The entity handle
		Uint32 m_group;		//!< The group the id belongs to

		bool operator==(const Entity::Id& other) const;
	};

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Entity();

	///////////////////////////////////////////////////////////
	/// \brief Construct the entity wrapper with a scene and id
	///
	///////////////////////////////////////////////////////////
	Entity(Scene* scene, Id id = Id());

	///////////////////////////////////////////////////////////
	/// \brief Get the entity id
	///
	/// \return An Entity::Id
	///
	///////////////////////////////////////////////////////////
	Id getId() const;

	///////////////////////////////////////////////////////////
	/// \brief Get a component pointer
	///
	/// \tparam C The component type
	///
	/// \return A pointer to the requested component
	///
	/// \see Scene::getComponent
	///
	///////////////////////////////////////////////////////////
	template <typename C>
	C* get() const;

	///////////////////////////////////////////////////////////
	/// \brief Check if the entity has a component type
	///
	/// \tparam C The component type
	///
	/// \return True if the entity has the requested component type
	///
	///////////////////////////////////////////////////////////
	template <typename C>
	bool has() const;

	///////////////////////////////////////////////////////////
	/// \brief Remove the current entity and all associated component data
	///
	/// \see Scene::removeEntity
	///
	///////////////////////////////////////////////////////////
	void remove() const;

	///////////////////////////////////////////////////////////
	/// \brief Add a tag to the entity
	///
	/// Tags should be an enum that can be cast to an integer.
	///
	/// \param tag The tag to add
	///
	/// \see Scene::addTag
	///
	///////////////////////////////////////////////////////////
	void addTag(int tag);

	///////////////////////////////////////////////////////////
	/// \brief Remove a tag from the entity
	///
	/// Tags should be an enum that can be cast to an integer.
	///
	/// \param tag The tag to remove
	///
	/// \see Scene::removeTag
	///
	///////////////////////////////////////////////////////////
	void removeTag(int tag);

	///////////////////////////////////////////////////////////
	/// \brief Check if the entity has a certain tag
	///
	/// Tags should be an enum that can be cast to an integer.
	///
	/// \param tag The tag to check for
	///
	/// \see Scene::hasTag
	///
	///////////////////////////////////////////////////////////
	bool hasTag(int tag) const;

private:
	Scene* m_scene;	//!< Scene pointer
	Id m_id;		//!< The entity id
};

}

#ifndef DOXYGEN_SKIP

namespace std
{

template <>
struct hash<poly::Entity::Id>
{
	std::size_t operator()(const poly::Entity::Id& k) const
	{
		return hash<poly::Uint64>()(*reinterpret_cast<const poly::Uint64*>(&k));
	}
};

}

#endif

#include <poly/Engine/Entity.inl>

#endif

///////////////////////////////////////////////////////////
/// \class poly::Entity
/// \ingroup Engine
///
/// Entity is a convenience wrapper for the Entity::Id and other
/// operations that involve the id.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// Scene scene;
///
/// // Create an entity with (int, float)
/// Entity e = scene.createEntity<int, float>(314, 3.14f);
///
/// // Component access
/// int* i = e.get<int>();
/// float* f = e.get<float>();
///
/// e.has<bool>(); // false
/// e.has<int>();  // true
///
///
/// // Add a tag
/// enum Tag
/// {
///		Player = 0,
///		Enemy
/// };
///
/// e.addTag(Tag::Player);
///
/// // Check for tags
/// e.hasTag(Tag::Player); // true
/// e.hasTag(Tag::Enemy);  // false
///
/// \endcode
///
///////////////////////////////////////////////////////////