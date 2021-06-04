#ifndef POLY_PHYSICS_COMPONENTS_H
#define POLY_PHYSICS_COMPONENTS_H

#include <poly/Math/Quaternion.h>
#include <poly/Math/Vector3.h>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief An enum describing a rigid body type
///
///////////////////////////////////////////////////////////
enum class RigidBodyType
{
	Static,			//!< A static type rigid body has infinite mass and moment of inertia, and can't have linear or angular velocity
	Kinematic,		//!< A kinematic rigid body type has infinite mass and moment of inertia, but is allowed to have linear and angular velocity
	Dynamic			//!< A dynamic rigid body type has finite mass and moment of inertia, and can have any linear and angular velocity
};


///////////////////////////////////////////////////////////
/// \brief A component containing properties for a rigid body
///
/// Rigid bodies are bodies that will be simulated by the laws
/// of physics during each update. They have mass, inertia, velocity,
/// etc. and the can be affected by forces and collisions.
///
///////////////////////////////////////////////////////////
struct RigidBodyComponent
{
	///////////////////////////////////////////////////////////
	/// \brief The default constructor
	///
	///////////////////////////////////////////////////////////
	RigidBodyComponent();

	///////////////////////////////////////////////////////////
	/// \brief Add a force at point in the rigid body's local coordinate system
	///
	/// This will apply a force at the specified point. This will
	/// change the body's linear and angular momentum by changing its
	/// total force and torque.
	///
	/// \param force The force to add
	/// \param point The point in local coordinate system to add the force at
	///
	///////////////////////////////////////////////////////////
	void addForceAtLocalPos(const Vector3f& force, const Vector3f& point);

	///////////////////////////////////////////////////////////
	/// \brief Add a force at point in the world coordinate system
	///
	/// This will apply a force at the specified point. This will
	/// change the body's linear and angular momentum by changing its
	/// total force and torque.
	///
	/// \param force The force to add
	/// \param point The point in world coordinate system to add the force at
	///
	///////////////////////////////////////////////////////////
	void addForceAtWorldPos(const Vector3f& force, const Vector3f& point);

	float m_mass;					//!< The mass
	Vector3f m_position;			//!< The position
	Quaternion m_rotation;			//!< The rotation orientation
	Vector3f m_linearVelocity;		//!< The linear velocity
	Vector3f m_angularVelocity;		//!< The angular velocity
	Vector3f m_centerOfMass;		//!< The location of the center of mass in local coordinate system
	Vector3f m_inertiaTensor;		//!< The inertia tensor (Describs the moment of inertia along the three rotational axes)
	float m_linearDamping;			//!< The damping factor to linear velocity
	float m_angularDamping;			//!< The damping factor to angular velocity
	Vector3f m_force;				//!< The total force to apply to the body during the next physics update, this value is reset to zero after every update
	Vector3f m_torque;				//!< The total torque to apply to the body during the next physics update, this value is reset to zero after every update
	RigidBodyType m_type;			//!< The type of the rigid body
};


///////////////////////////////////////////////////////////
/// \brief A component containing properties for a collision body
///
/// A collision body can detect any collisions that occur with itself,
/// but it is not affected by collisions. It has no mass, inertia,
/// velocity, etc. so it cannot be affected by the laws of physics.
/// It is used mostly to attach colliders to in order to detect
/// collisions.
///
///////////////////////////////////////////////////////////
struct CollisionBodyComponent
{
	CollisionBodyComponent();

	Vector3f m_position;			//!< The position
	Quaternion m_rotation;			//!< The rotation orientation
};


}

#endif