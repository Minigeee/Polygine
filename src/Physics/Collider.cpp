#include <poly/Core/Logger.h>

#include <poly/Physics/Collider.h>

#include <reactphysics3d/reactphysics3d.h>

#define COLLIDER_CAST(x) reinterpret_cast<reactphysics3d::Collider*>(x)
#define MATERIAL_CAST(x) reinterpret_cast<reactphysics3d::Material*>(x)

#define RP3D_VEC3(v) reactphysics3d::Vector3(v.x, v.y, v.z)
#define RP3D_QUAT(q) reactphysics3d::Quaternion(q.x, q.y, q.z, q.w)
#define POLY_VEC3(v) poly::Vector3f(v.x, v.y, v.z)
#define POLY_QUAT(q) poly::Quaternion(q.x, q.y, q.z, q.w)

namespace poly
{


///////////////////////////////////////////////////////////
Collider::Collider() :
	m_collider				(0),
	m_material				(0),
	m_shape					(0)
{

}


///////////////////////////////////////////////////////////
Collider::~Collider()
{

}


///////////////////////////////////////////////////////////
void Collider::setPosition(const Vector3f& pos)
{
	ASSERT(m_collider, "The collider must be created using the physics scene extension");

	reactphysics3d::Transform transform = COLLIDER_CAST(m_collider)->getLocalToBodyTransform();
	transform.setPosition(RP3D_VEC3(pos));
	COLLIDER_CAST(m_collider)->setLocalToBodyTransform(transform);
}


///////////////////////////////////////////////////////////
void Collider::setPosition(float x, float y, float z)
{
	setPosition(Vector3f(x, y, z));
}


///////////////////////////////////////////////////////////
void Collider::setRotation(const Quaternion& rotation)
{
	ASSERT(m_collider, "The collider must be created using the physics scene extension");

	reactphysics3d::Transform transform = COLLIDER_CAST(m_collider)->getLocalToBodyTransform();
	transform.setOrientation(RP3D_QUAT(rotation));
	COLLIDER_CAST(m_collider)->setLocalToBodyTransform(transform);
}


///////////////////////////////////////////////////////////
void Collider::setBounciness(float bounciness)
{
	ASSERT(m_material, "The collider must be created using the physics scene extension");
	MATERIAL_CAST(m_material)->setBounciness(bounciness);
}


///////////////////////////////////////////////////////////
void Collider::setFrictionCoefficient(float coefficient)
{
	ASSERT(m_material, "The collider must be created using the physics scene extension");
	MATERIAL_CAST(m_material)->setFrictionCoefficient(coefficient);
}


///////////////////////////////////////////////////////////
void Collider::setRollingResistance(float resistance)
{
	ASSERT(m_material, "The collider must be created using the physics scene extension");
	MATERIAL_CAST(m_material)->setRollingResistance(resistance);
}


///////////////////////////////////////////////////////////
void Collider::setCollisionCategory(Uint16 category)
{
	ASSERT(m_collider, "The collider must be created using the physics scene extension");
	COLLIDER_CAST(m_collider)->setCollisionCategoryBits(category);
}


///////////////////////////////////////////////////////////
void Collider::setCollisionMask(Uint16 mask)
{
	ASSERT(m_collider, "The collider must be created using the physics scene extension");
	COLLIDER_CAST(m_collider)->setCollideWithMaskBits(mask);
}


///////////////////////////////////////////////////////////
void Collider::setIsTrigger(bool trigger)
{
	ASSERT(m_collider, "The collider must be created using the physics scene extension");
	COLLIDER_CAST(m_collider)->setIsTrigger(trigger);
}


///////////////////////////////////////////////////////////
float Collider::getBounciness() const
{
	ASSERT(m_material, "The collider must be created using the physics scene extension");
	return MATERIAL_CAST(m_material)->getBounciness();
}


///////////////////////////////////////////////////////////
float Collider::getFrictionCoefficient() const
{
	ASSERT(m_material, "The collider must be created using the physics scene extension");
	return MATERIAL_CAST(m_material)->getFrictionCoefficient();
}


///////////////////////////////////////////////////////////
float Collider::getRollingResistance() const
{
	ASSERT(m_material, "The collider must be created using the physics scene extension");
	return MATERIAL_CAST(m_material)->getRollingResistance();
}


///////////////////////////////////////////////////////////
Uint16 Collider::getCollisionCategory() const
{
	ASSERT(m_collider, "The collider must be created using the physics scene extension");
	return COLLIDER_CAST(m_collider)->getCollisionCategoryBits();
}


///////////////////////////////////////////////////////////
Uint16 Collider::getCollisionMask() const
{
	ASSERT(m_collider, "The collider must be created using the physics scene extension");
	return COLLIDER_CAST(m_collider)->getCollideWithMaskBits();
}


///////////////////////////////////////////////////////////
bool Collider::isTrigger() const
{
	ASSERT(m_collider, "The collider must be created using the physics scene extension");
	return COLLIDER_CAST(m_collider)->getIsTrigger();
}


///////////////////////////////////////////////////////////
void Collider::init(void* collider)
{
	m_collider = collider;
	m_material = &COLLIDER_CAST(collider)->getMaterial();
	m_shape = COLLIDER_CAST(collider)->getCollisionShape();

	MATERIAL_CAST(m_material)->setBounciness(0.1f);
	MATERIAL_CAST(m_material)->setFrictionCoefficient(0.2f);
}


}