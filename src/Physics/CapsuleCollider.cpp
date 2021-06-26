#include <poly/Core/Logger.h>

#include <poly/Physics/CapsuleCollider.h>

#include <reactphysics3d/reactphysics3d.h>

#define COLLIDER_CAST(x) reinterpret_cast<reactphysics3d::Collider*>(x)
#define SHAPE_CAST(x) reinterpret_cast<reactphysics3d::CapsuleShape*>(x)

#define RP3D_VEC3(v) reactphysics3d::Vector3(v.x, v.y, v.z)
#define RP3D_QUAT(q) reactphysics3d::Quaternion(q.x, q.y, q.z, q.w)
#define POLY_VEC3(v) poly::Vector3f(v.x, v.y, v.z)
#define POLY_QUAT(q) poly::Quaternion(q.x, q.y, q.z, q.w)

namespace poly
{


///////////////////////////////////////////////////////////
void CapsuleCollider::setRadius(float radius)
{
	ASSERT(m_shape, "The collider must be created using the physics scene extension");
	SHAPE_CAST(m_shape)->setRadius(radius);

	awakenBody();
}


///////////////////////////////////////////////////////////
void CapsuleCollider::setHeight(float height)
{
	ASSERT(m_shape, "The collider must be created using the physics scene extension");
	SHAPE_CAST(m_shape)->setHeight(height);

	awakenBody();
}


///////////////////////////////////////////////////////////
float CapsuleCollider::getRadius() const
{
	ASSERT(m_shape, "The collider must be created using the physics scene extension");
	return SHAPE_CAST(m_shape)->getRadius();
}


///////////////////////////////////////////////////////////
float CapsuleCollider::getHeight() const
{
	ASSERT(m_shape, "The collider must be created using the physics scene extension");
	return SHAPE_CAST(m_shape)->getHeight();
}


}