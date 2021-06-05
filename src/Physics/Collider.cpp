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

	reactphysics3d::Material& material = rp3d->getMaterial();
	material.setBounciness(m_bounciness);
	material.setFrictionCoefficient(m_frictionCoefficient);
	material.setRollingResistance(m_rollingResistance);
}


}