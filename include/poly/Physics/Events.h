#ifndef POLY_PHYSICS_EVENTS_H
#define POLY_PHYSICS_EVENTS_H

#include <poly/Engine/Entity.h>

#include <poly/Physics/Collider.h>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief An enum for collision event types
///
///////////////////////////////////////////////////////////
enum class CollisionEventType
{
	Start,		//!< Indicates when two colliders first come into contact
	End			//!< Indicates when two colliders leave contact
};


///////////////////////////////////////////////////////////
/// \brief A struct containing data on collision contact points
///
///////////////////////////////////////////////////////////
struct ContactPoint
{
	float m_penetrationDepth;		//!< The penetration depth between the two colliders
	Vector3f m_normal;				//!< The normal of the contact point, in world space coordinate system
	Vector3f m_pointOnColliders[2];	//!< The local position of the contact points on the collider objects
};


///////////////////////////////////////////////////////////
/// \brief An event that is sent when two collider objects collide
/// \ingroup Events
///
///////////////////////////////////////////////////////////
struct E_PhysicsCollision
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	E_PhysicsCollision() :
		m_type			(CollisionEventType::Start),
		m_numContacts	(0),
		m_contacts		(0)
	{ }

	CollisionEventType m_type;	//!< The collision event type
	Entity::Id m_entities[2];	//!< The id of the entities involved in the collision event
	Collider m_colliders[2];	//!< The collider objects involved in the collision event
	Uint32 m_numContacts;		//!< The number of contact points in the collision
	ContactPoint* m_contacts;	//!< The list of contact points
};


///////////////////////////////////////////////////////////
/// \brief An event that is sent when a collider collides with a trigger collider object
/// \ingroup Events
///
///////////////////////////////////////////////////////////
struct E_PhysicsTrigger
{
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	E_PhysicsTrigger() :
		m_type			(CollisionEventType::Start)
	{ }

	CollisionEventType m_type;	//!< The trigger collision event type
	Entity::Id m_entities[2];	//!< The id of the entities involved in the collision event
	Collider m_colliders[2];	//!< The collider objects involved in the collision event
};


}

#endif