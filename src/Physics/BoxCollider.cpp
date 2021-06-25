#include <poly/Core/Logger.h>

#include <poly/Physics/BoxCollider.h>

#include <reactphysics3d/reactphysics3d.h>

#define SHAPE_CAST(x) reinterpret_cast<reactphysics3d::BoxShape*>(x)

#define RP3D_VEC3(v) reactphysics3d::Vector3(v.x, v.y, v.z)
#define RP3D_QUAT(q) reactphysics3d::Quaternion(q.x, q.y, q.z, q.w)
#define POLY_VEC3(v) poly::Vector3f(v.x, v.y, v.z)
#define POLY_QUAT(q) poly::Quaternion(q.x, q.y, q.z, q.w)

namespace poly
{


///////////////////////////////////////////////////////////
void BoxCollider::setDimensions(const Vector3f& dims)
{
	ASSERT(m_shape, "The collider must be created using the physics scene extension");

	Vector3f halfExtents = 0.5f * dims;
	SHAPE_CAST(m_shape)->setHalfExtents(RP3D_VEC3(halfExtents));
}


///////////////////////////////////////////////////////////
void BoxCollider::setDimensions(float x, float y, float z)
{
	setDimensions(Vector3f(x, y, z));
}


///////////////////////////////////////////////////////////
Vector3f BoxCollider::getDimensions() const
{
	ASSERT(m_shape, "The collider must be created using the physics scene extension");

	reactphysics3d::Vector3 dims = 2.0f * SHAPE_CAST(m_shape)->getHalfExtents();
	return POLY_VEC3(dims);
}


}