#include <poly/Physics/Collider.h>

#include <reactphysics3d/reactphysics3d.h>

#define COLLIDER_CAST(x) reinterpret_cast<reactphysics3d::Collider*>(x)

namespace poly
{


///////////////////////////////////////////////////////////
Collider::Collider() :
	m_collider				(0),
	m_bounciness			(0.1f),
	m_frictionCoefficient	(0.2f),
	m_rollingResistance		(0.0f),
	m_collisionCategory		(1),
	m_collisionMask			(0xFFFF),
	m_isTrigger				(false)
{

}


///////////////////////////////////////////////////////////
void Collider::setBounciness(float bounciness)
{
	m_bounciness = bounciness;
	if (m_collider)
		COLLIDER_CAST(m_collider)->getMaterial().setBounciness(m_bounciness);
}


///////////////////////////////////////////////////////////
void Collider::setFrictionCoefficient(float coefficient)
{
	m_frictionCoefficient = coefficient;
	if (m_collider)
		COLLIDER_CAST(m_collider)->getMaterial().setFrictionCoefficient(m_frictionCoefficient);
}


///////////////////////////////////////////////////////////
void Collider::setRollingResistance(float resistance)
{
	m_rollingResistance = resistance;
	if (m_collider)
		COLLIDER_CAST(m_collider)->getMaterial().setRollingResistance(m_rollingResistance);
}


///////////////////////////////////////////////////////////
void Collider::setCollisionCategory(Uint16 category)
{
	m_collisionCategory = category;
	if (m_collider)
		COLLIDER_CAST(m_collider)->setCollisionCategoryBits(m_collisionCategory);
}


///////////////////////////////////////////////////////////
void Collider::setCollisionMask(Uint16 mask)
{
	m_collisionMask = mask;
	if (m_collider)
		COLLIDER_CAST(m_collider)->setCollideWithMaskBits(m_collisionMask);
}


///////////////////////////////////////////////////////////
void Collider::setIsTrigger(bool trigger)
{
	m_isTrigger = trigger;
	if (m_collider)
		COLLIDER_CAST(m_collider)->setIsTrigger(m_isTrigger);
}


///////////////////////////////////////////////////////////
float Collider::getBounciness() const
{
	return m_bounciness;
}


///////////////////////////////////////////////////////////
float Collider::getFrictionCoefficient() const
{
	return m_frictionCoefficient;
}


///////////////////////////////////////////////////////////
float Collider::getRollingResistance() const
{
	return m_rollingResistance;
}


///////////////////////////////////////////////////////////
Uint16 Collider::getCollisionCategory() const
{
	return m_collisionCategory;
}


///////////////////////////////////////////////////////////
Uint16 Collider::getCollisionMask() const
{
	return m_collisionMask;
}


///////////////////////////////////////////////////////////
bool Collider::isTrigger() const
{
	return m_isTrigger;
}


///////////////////////////////////////////////////////////
void Collider::setCollider(void* collider)
{
	m_collider = collider;

	reactphysics3d::Collider* rp3d = COLLIDER_CAST(collider);
	rp3d->setIsTrigger(m_isTrigger);
	rp3d->setCollisionCategoryBits(m_collisionCategory);
	rp3d->setCollideWithMaskBits(m_collisionMask);

	reactphysics3d::Material& material = rp3d->getMaterial();
	material.setBounciness(m_bounciness);
	material.setFrictionCoefficient(m_frictionCoefficient);
	material.setRollingResistance(m_rollingResistance);
}


}