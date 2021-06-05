#ifndef POLY_COLLIDER_H
#define POLY_COLLIDER_H

#include <poly/Math/Quaternion.h>
#include <poly/Math/Vector3.h>

namespace poly
{


class Physics;
class PhysicsEventHandler;


class Collider
{
	friend Physics;
	friend PhysicsEventHandler;

public:
	Collider();

	void setBounciness(float bounciness);

	void setFrictionCoefficient(float coefficient);

	void setRollingResistance(float resistance);

	void setCollisionCategory(Uint16 category);

	void setCollisionMask(Uint16 mask);

	void setIsTrigger(bool trigger);

	float getBounciness() const;

	float getFrictionCoefficient() const;

	float getRollingResistance() const;

	Uint16 getCollisionCategory() const;

	Uint16 getCollisionMask() const;

	bool isTrigger() const;

private:
	void setCollider(void* collider);

private:
	void* m_collider;
	float m_bounciness;
	float m_frictionCoefficient;
	float m_rollingResistance;
	Uint16 m_collisionCategory;
	Uint16 m_collisionMask;
	bool m_isTrigger;
};


}

#endif