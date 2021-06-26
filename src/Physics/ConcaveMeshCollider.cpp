#include <poly/Core/Logger.h>

#include <poly/Physics/ConcaveMeshCollider.h>

#include <reactphysics3d/reactphysics3d.h>

#define SHAPE_CAST(x) reinterpret_cast<reactphysics3d::ConcaveMeshShape*>(x)

#define RP3D_VEC3(v) reactphysics3d::Vector3(v.x, v.y, v.z)
#define RP3D_QUAT(q) reactphysics3d::Quaternion(q.x, q.y, q.z, q.w)
#define POLY_VEC3(v) poly::Vector3f(v.x, v.y, v.z)
#define POLY_QUAT(q) poly::Quaternion(q.x, q.y, q.z, q.w)

namespace poly
{


///////////////////////////////////////////////////////////
void ConcaveMeshCollider::setScale(const Vector3f& scale)
{
	ASSERT(m_shape, "The collider must be created using the physics scene extension");
	SHAPE_CAST(m_shape)->setScale(RP3D_VEC3(scale));

	awakenBody();
}


///////////////////////////////////////////////////////////
void ConcaveMeshCollider::setScale(float x, float y, float z)
{
	setScale(Vector3f(x, y, z));
}


///////////////////////////////////////////////////////////
Vector3f ConcaveMeshCollider::getScale() const
{
	ASSERT(m_shape, "The collider must be created using the physics scene extension");

	const reactphysics3d::Vector3& scale = SHAPE_CAST(m_shape)->getScale();
	return POLY_VEC3(scale);
}


}