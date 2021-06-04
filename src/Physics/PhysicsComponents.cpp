#include <poly/Math/Transform.h>

#include <poly/Physics/Components.h>

namespace poly
{


///////////////////////////////////////////////////////////
RigidBodyComponent::RigidBodyComponent() :
	m_mass				(1.0f),
	m_position			(0.0f),
	m_linearVelocity	(0.0f),
	m_angularVelocity	(0.0f),
	m_centerOfMass		(0.0f),
	m_inertiaTensor		(1.0f),
	m_linearDamping		(0.0f),
	m_angularDamping	(0.0f),
	m_force				(0.0f),
	m_torque			(0.0f),
	m_type				(RigidBodyType::Dynamic)
{

}


///////////////////////////////////////////////////////////
void RigidBodyComponent::addForceAtLocalPos(const Vector3f& force, const Vector3f& point)
{
	// Add force
	m_force += force;

	// Add torque
	Vector3f centerOfMass = Vector3f(toTransformMatrix(Vector3f(0.0f), m_rotation, Vector3f(1.0f)) * Vector4f(m_centerOfMass, 1.0f));
	m_torque += cross(point - centerOfMass, force);
}


///////////////////////////////////////////////////////////
void RigidBodyComponent::addForceAtWorldPos(const Vector3f& force, const Vector3f& point)
{
	// Add force
	m_force += force;

	// Add torque
	Vector3f worldCenterOfMass = Vector3f(toTransformMatrix(m_position, m_rotation, Vector3f(1.0f)) * Vector4f(m_centerOfMass, 1.0f));
	m_torque += cross(point - worldCenterOfMass, force);
}


///////////////////////////////////////////////////////////
CollisionBodyComponent::CollisionBodyComponent() :
	m_position			(0.0f)
{

}


}