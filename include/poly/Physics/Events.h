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
	Vector3f m_pointOnCollider1;	//!< The local position of the contact point on the first collider object
	Vector3f m_pointOnCollider2;	//!< The local position of the contact point on the second collider object
};


///////////////////////////////////////////////////////////
/// \brief An event that is sent when two collider objects collide
///
///////////////////////////////////////////////////////////
struct E_PhysicsCollision
{
	E_PhysicsCollision() :
		m_type			(CollisionEventType::Start),
		m_numContacts	(0),
		m_contacts		(0)
	{ }

	CollisionEventType m_type;	//!< The collision event type
	Entity::Id m_entity1;		//!< The id of the first entity involved in the collision event
	Entity::Id m_entity2;		//!< The id of the second entity involved in the collision event
	Collider m_collider1;		//!< The first collider object involved in the collision event
	Collider m_collider2;		//!< The second collider object involved in the collision event
	Uint32 m_numContacts;		//!< The number of contact points in the collision
	ContactPoint* m_contacts;	//!< The list of contact points
};


///////////////////////////////////////////////////////////
/// \brief An event that is sent when a collider collides with a trigger collider object
///
///////////////////////////////////////////////////////////
struct E_PhysicsTrigger
{
	E_PhysicsTrigger() :
		m_type			(CollisionEventType::Start)
	{ }

	CollisionEventType m_type;	//!< The trigger collision event type
	Entity::Id m_entity1;		//!< The id of the first entity involved in the collision event
	Entity::Id m_entity2;		//!< The id of the second entity involved in the collision event
	Collider m_collider1;		//!< The first collider object involved in the collision event
	Collider m_collider2;		//!< The second collider object involved in the collision event
};


}

#endif