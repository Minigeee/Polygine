#ifndef POLY_PHYSICS_COMPONENTS_H
#define POLY_PHYSICS_COMPONENTS_H

#include <poly/Math/Quaternion.h>
#include <poly/Math/Vector3.h>

namespace poly
{


enum class RigidBodyType
{
	Static,
	Kinematic,
	Dynamic
};


struct RigidBodyComponent
{
	RigidBodyComponent();

	void addForceAtLocalPos(const Vector3f& force, const Vector3f& point);

	void addForceAtWorldPos(const Vector3f& force, const Vector3f& point);

	float m_mass;
	Vector3f m_position;
	Quaternion m_rotation;
	Vector3f m_linearVelocity;
	Vector3f m_angularVelocity;
	Vector3f m_centerOfMass;
	Vector3f m_inertiaTensor;
	float m_linearDamping;
	float m_angularDamping;
	Vector3f m_force;
	Vector3f m_torque;
	RigidBodyType m_type;
};


}

#endif