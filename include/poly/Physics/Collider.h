#ifndef POLY_COLLIDER_H
#define POLY_COLLIDER_H

#include <poly/Math/Quaternion.h>
#include <poly/Math/Vector3.h>

namespace poly
{


class Physics;


class Collider
{
	friend Physics;

public:
	Collider();

	void setBounciness(float bounciness);

	void setFrictionCoefficient(float coefficient);

	void setRollingResistance(float resistance);

	float getBounciness() const;

	float getFrictionCoefficient() const;

	float getRollingResistance() const;

private:
	void setCollider(void* collider);

private:
	void* m_collider;
	float m_bounciness;
	float m_frictionCoefficient;
	float m_rollingResistance;
};


}

#endif